#include "runtime.h"
#include "gfx/canvas.h"
#include "gfx/theme.h"
#include "storage/shelf.h"
#include "util/crc32.h"
#include "platform.h"
#include <stdio.h>
#include <string.h>


namespace {
constexpr uint32_t kNativeFrameUs = 16667;
}  // namespace

void Runtime::begin() {
  UsbPad::attach();  // before anything allocates: the USB descriptors are built on the heap
  display_.begin();
  input_.begin();
  link_.begin();
  storage_.begin();
  Link::sweepTemp(storage_);
  if (!shelf::usable()) lastError_ = "FIRMWARE TOO BIG FOR SHELF";
  uint8_t cores = 0;
  const ContentType systems[4] = {ContentType::Atari2600, ContentType::NES, ContentType::GB, ContentType::GBC};
  for (int i = 0; i < 4; ++i) if (hasCore(systems[i])) cores |= (uint8_t)(1u << i);
  ui_.setEmulatorCores(cores);
  loadSettings();
  ui_.begin();
  if (lastError_[0]) ui_.notify(lastError_, plat_now_us());
  mode_ = Mode::Idle;
}

void Runtime::stop() {
  // A settings change made just before starting a game, or a new arcade high score.
  if (settingsChangedUs_ || mode_ == Mode::Arcade) saveSettings();
  if (mode_ == Mode::Neko) saveNeko();
  if (mode_ == Mode::Gb) { saveGb(); gb_.stop(); }
  if (mode_ == Mode::Nes) { saveNes(); nes_.stop(); }
  if (mode_ == Mode::Atari) atari_.stop();
  if (mode_ == Mode::Gamepad) pad_.end();
  rom_.close();
  mode_ = Mode::Idle;
}

void Runtime::fail(const char* why) {
  lastError_ = why;
  stop();
}

static const char* const kNekoSave = "/saves/cat.sav";
static const char* const kSystemSave = "/saves/system.sav";


void Runtime::drawInstall(uint32_t done, uint32_t total) {
  Canvas c{renderer_.fb(), Display::W, Display::H};
  const Theme& t = theme();
  c.fill(0, 0, Display::W, Display::H, t.bg);
  c.fill(0, 0, Display::W, 18, t.accent);
  c.text(8, 6, "INSTALLING ROM", t.ink);
  const char* base = strrchr(gbPath_, '/');
  c.textClip(8, 60, base ? base + 1 : gbPath_, t.text, 34);
  c.fill(8, 76, 204, 16, t.text);  // 2 px frame, then the bar
  c.fill(10, 78, 200, 12, t.bg);
  c.fill(12, 80, total ? (int)(196ull * done / total) : 0, 8, t.accent);
  char pct[8];
  snprintf(pct, sizeof pct, "%u%%", total ? (unsigned)(100ull * done / total) : 0u);
  c.textRight(212, 100, pct, t.text);
  c.text(8, 100, "FIRST LAUNCH ONLY", t.muted);
  renderer_.presentDiff();
}

// ROMs on the shelf run in place (XIP). A ROM that is a plain file (SD card) is copied onto the
// shelf once, and again only if the file changes.
const uint8_t* Runtime::installRom(const char* path) {
  snprintf(gbPath_, sizeof gbPath_, "%s", path);
  if (const uint8_t* p = rom_.map()) return p;
  const shelf::Entry* e = shelf::find(path);
  if (e && e->size == rom_.size() && e->sig == shelf::signature(path, rom_)) return shelf::data(e);
  renderer_.invalidateDiff();
  drawInstall(0, rom_.size());
  e = shelf::install(path, rom_, [](uint32_t d, uint32_t t, void* self) { ((Runtime*)self)->drawInstall(d, t); }, this);
  if (!e) fail(rom_.size() > shelf::kCapacity ? "ROM TOO LARGE" : rom_.size() > shelf::largestFree() ? "SHELF FULL" : "ROM INSTALL FAILED");
  return e ? shelf::data(e) : nullptr;
}

bool Runtime::startGb(const char* path, ContentType type) {
  gbType_ = type;
  const uint8_t* rom = installRom(path);
  if (!rom) return false;
  const uint32_t size = rom_.size();
  rom_.close();  // the emulator reads the flash slot from now on
  if (const char* e = gb_.start(rom, size)) { fail(e); return false; }
  // Battery save
  char sp[96];
  Storage::savePath(sp, sizeof sp, gbType_, path);
  FsRomSource sf;
  if (gb_.saveSize() && storage_.openRom(sp, sf)) gb_.loadSave(sf);
  sf.close();
  gbDirtySinceUs_ = 0;
  mode_ = Mode::Gb;
  nativeDueUs_ = plat_now_us();
  return true;
}


bool Runtime::startAtari() {  // the ROM (<= 32 KB) is copied to RAM; the flash ROM slot is untouched
  const char* e = atari_.start(rom_);
  rom_.close();
  if (e) { fail(e); return false; }
  mode_ = Mode::Atari;
  nativeDueUs_ = plat_now_us();
  return true;
}


bool Runtime::startNes(const char* path) {
  const uint8_t* rom = installRom(path);
  if (!rom) return false;
  const uint32_t size = rom_.size();
  rom_.close();
  if (const char* e = nes_.start(rom, size)) { fail(e); return false; }
  char sp[96];
  Storage::savePath(sp, sizeof sp, ContentType::NES, path);
  FsRomSource sf;
  if (nes_.hasBattery() && storage_.openRom(sp, sf)) nes_.loadSave(sf);
  sf.close();
  gbDirtySinceUs_ = 0;
  mode_ = Mode::Nes;
  nativeDueUs_ = plat_now_us();
  return true;
}

void Runtime::saveNes() {
  if (!nes_.running() || !nes_.hasBattery()) return;
  static uint32_t savedCrc = 0;
  const uint32_t crc = piko::crc32(nes_.saveData(), nes_.saveSize());
  if (crc == savedCrc) { nes_.clearSaveDirty(); return; }
  fs::FS* fsw = storage_.writeFs();
  if (!fsw) return;
  char sp[96];
  Storage::savePath(sp, sizeof sp, ContentType::NES, gbPath_);
  fsw->mkdir("/saves/nes");
  fs::File f = fsw->open(sp, "w");
  if (!f) return;
  f.write(nes_.saveData(), nes_.saveSize());
  f.close();
  nes_.clearSaveDirty();
  savedCrc = crc;
}

void Runtime::saveGb() {
  if (!gb_.running() || !gb_.saveSize() || (!gb_.saveDirty() && !gb_.hasRtc())) return;
  // Skip the write when the RAM matches what is already saved (scratch use that was undone).
  static uint32_t savedCrc = 0;
  uint32_t crc = piko::crc32(gb_.saveData(), gb_.saveSize());
  if (gb_.hasRtc()) { uint8_t tail[GbcPlayer::kRtcTail]; gb_.rtcTail(tail); crc ^= piko::crc32(tail, sizeof tail) * 31u; }
  if (crc == savedCrc) { gb_.clearSaveDirty(); return; }
  fs::FS* fsw = storage_.writeFs();
  if (!fsw) return;
  char sp[96];
  Storage::savePath(sp, sizeof sp, gbType_, gbPath_);
  fsw->mkdir(gbType_ == ContentType::GBC ? "/saves/gbc" : "/saves/gb");
  fs::File f = fsw->open(sp, "w");
  if (!f) return;
  f.write(gb_.saveData(), gb_.saveSize());
  if (gb_.hasRtc()) {
    uint8_t tail[GbcPlayer::kRtcTail];
    gb_.rtcTail(tail);
    f.write(tail, sizeof tail);
  }
  f.close();
  gb_.clearSaveDirty();
  savedCrc = crc;
}

// GAMEPAD: the console becomes a USB HID gamepad until SELECT+START is held for a second.
void Runtime::startGamepad() {
  stop();
  pad_.begin();
  padView_.invalidate();
  mode_ = Mode::Gamepad;
}

void Runtime::startNeko() {
  stop();
  CatPet::Save sv{};
  bool have = false;
  FsRomSource f;
  if (storage_.openRom(kNekoSave, f) && f.size() == sizeof sv) have = f.read(0, &sv, sizeof sv) == sizeof sv;
  f.close();
  neko_.start((uint32_t)plat_now_us(), have ? &sv : nullptr);
  renderer_.invalidateDiff();
  mode_ = Mode::Neko;
  nativeDueUs_ = nekoSavedUs_ = plat_now_us();
}

// The cat lives in /saves/cat.sav (tiny). Saved when leaving NEKO and once a minute while playing.
void Runtime::saveNeko() {
  fs::FS* fsw = storage_.writeFs();
  if (!fsw) return;
  const CatPet::Save sv = neko_.save();
  fs::File f = fsw->open(kNekoSave, "w");
  if (!f) return;
  f.write((const uint8_t*)&sv, sizeof sv);
  f.close();
}

static_assert(Arcade::kGameCount <= 4, "system.sav keeps 4 high scores: add a new format version (\"PSY3\") for more games");

Runtime::SystemSave Runtime::settingsNow() const {
  SystemSave s{};
  memcpy(s.magic, "PSY2", 4);
  s.theme = g_themeIndex;
  s.gbScale = g_gbScale;
  s.gbPalette = g_gbPalette;
  s.menuSpeed = g_menuSpeed;
  s.hideEmpty = g_hideEmpty;
  s.padStick = g_padStick;
  for (int i = 0; i < Arcade::kGameCount; ++i) s.best[i] = arcade_.best((Arcade::Game)i);
  return s;
}

void Runtime::loadSettings() {
  SystemSave s{};
  FsRomSource f;
  bool ok = false;
  if (storage_.openRom(kSystemSave, f)) {
    uint8_t b[sizeof s] = {};
    const uint32_t n = f.size();
    if (n == sizeof s && f.read(0, b, n) == n && !memcmp(b, "PSY2", 4)) {
      memcpy(&s, b, sizeof s);
      ok = true;
    } else if (n == 24 && f.read(0, b, n) == n && !memcmp(b, "PSY1", 4)) {  // 0.3: DARK/LIGHT, GB SCREEN
      s.theme = b[4] & 1;                                                     // DARK -> MONO, LIGHT -> PAPER
      s.gbScale = b[5];
      s.menuSpeed = 1;
      memcpy(s.best, b + 8, sizeof s.best);
      ok = true;
    }
  }
  f.close();
  if (ok) {
    g_themeIndex = s.theme % kThemeCount;
    g_gbScale = s.gbScale & 1;
    g_gbPalette = s.gbPalette % kGbPaletteCount;
    g_menuSpeed = s.menuSpeed % kMenuSpeedCount;
    g_hideEmpty = s.hideEmpty & 1;
    g_padStick = s.padStick & 1;
    for (int i = 0; i < Arcade::kGameCount; ++i) arcade_.setBest((Arcade::Game)i, (int)s.best[i]);
  }
  applySettings();
  settingsSaved_ = settingsNow();
  settingsChangedUs_ = 0;
}

void Runtime::applySettings() {
  static const uint32_t kDelay[kMenuSpeedCount] = {500000, 350000, 240000};
  static const uint32_t kInterval[kMenuSpeedCount] = {140000, 90000, 50000};
  input_.setRepeat(kDelay[g_menuSpeed % kMenuSpeedCount], kInterval[g_menuSpeed % kMenuSpeedCount]);
  int best[Arcade::kGameCount];
  for (int i = 0; i < Arcade::kGameCount; ++i) best[i] = arcade_.best((Arcade::Game)i);
  ui_.setArcadeBest(best);
}

void Runtime::saveSettings() {
  settingsChangedUs_ = 0;
  const SystemSave s = settingsNow();
  if (!memcmp(&s, &settingsSaved_, sizeof s)) return;
  fs::FS* fsw = storage_.writeFs();
  if (!fsw) return;
  fsw->mkdir("/saves");
  fs::File f = fsw->open(kSystemSave, "w");
  if (!f) return;
  const bool ok = f.write((const uint8_t*)&s, sizeof s) == sizeof s;
  f.close();
  if (ok) settingsSaved_ = s;
}

void Runtime::settingsTick(uint64_t now) {
  const SystemSave s = settingsNow();
  if (!memcmp(&s, &settingsSaved_, sizeof s)) { settingsChangedUs_ = 0; return; }
  if (!settingsChangedUs_) settingsChangedUs_ = now | 1;
  else if (now - settingsChangedUs_ >= 1000000) saveSettings();
}

bool Runtime::startContent(ContentType t) {
  if (t == ContentType::Unknown) { fail("UNKNOWN FILE TYPE"); return false; }
  if (!hasCore(t)) { fail("NO EMULATOR CORE INSTALLED"); return false; }
  if (t == ContentType::GB || t == ContentType::GBC) return startGb(currentPath_, t);
  if (t == ContentType::NES) return startNes(currentPath_);
  return startAtari();
}

bool Runtime::launch(const char* path) {
  if (!path || !path[0]) return false;
  stop();
  lastError_ = "";
  if (!storage_.openRom(path, rom_)) { fail("CANNOT OPEN FILE"); return false; }
  currentPath_ = path;
  const bool ok = startContent(detectContent(path, rom_));
  if (ok) ui_.close();  // the game owns the screen and the main loop now
  return ok;
}

void Runtime::core1() {
  if (mode_ == Mode::Gb) gb_.presentService();
  else if (mode_ == Mode::Nes) nes_.presentService();
  else if (mode_ == Mode::Atari) atari_.presentService();
  else plat_sleep_us(500);
}

void Runtime::sleepUntil(uint64_t due) {
  uint64_t n = plat_now_us();
  if (due > n + 1500) plat_sleep_us((uint32_t)(due - n - 1000));
}

void Runtime::loop() {
  uint64_t now = plat_now_us();
  link_.poll(now, storage_, mode_ != Mode::Idle || !ui_.active());
  input_.poll(now);
  // One logical snapshot for everyone. MENU is the runtime's; consumers never see it.
  InputState in = input_.state();
  const uint16_t both = BTN_SELECT | BTN_START;
  const bool chord = (in.held & both) == both;
  if (chord && !menuHold_) { menuHold_ = true; menuHoldUs_ = now; }
  if (!chord) menuHold_ = false;
  const bool menu = menuHold_ && now - menuHoldUs_ >= kMenuHoldUs;
  in.held &= (uint16_t)~BTN_MENU;
  in.pressed &= (uint16_t)~BTN_MENU;
  in.released &= (uint16_t)~BTN_MENU;
  in.repeated &= (uint16_t)~BTN_MENU;

  {  // PikoLink LAUNCH: start a file directly from a PC
    char lp[64];
    if (link_.takeLaunch(lp, sizeof lp)) {
      stop();
      if (!launch(lp)) { ui_.open(); ui_.notify(lastError_, now); }
    }
  }
  accPressed_ |= in.pressed;
  accReleased_ |= in.released;
  accRepeated_ |= in.repeated;

  if (menu && !ui_.active()) {
    stop();
    ui_.open();
    menuHold_ = false;
    accPressed_ = accReleased_ = accRepeated_ = 0;
    now = plat_now_us();
  }

  if (ui_.active()) {
    settingsTick(now);
    applySettings();
    const LauncherAction a = ui_.frame(in, now, storage_);
    // Presses that drove the menu must not leak into the first frame of the game they started.
    if (a.kind != LauncherAction::Kind::None) accPressed_ = accReleased_ = accRepeated_ = 0;
    if (a.kind == LauncherAction::Kind::ClearScores) {
      for (int i = 0; i < Arcade::kGameCount; ++i) arcade_.setBest((Arcade::Game)i, 0);
    } else if (a.kind == LauncherAction::Kind::PlayRom) {
      if (!launch(a.path)) { ui_.open(); ui_.notify(lastError_, now); }
    } else if (a.kind == LauncherAction::Kind::PlayNeko) {
      startNeko();
    } else if (a.kind == LauncherAction::Kind::Gamepad) {
      startGamepad();
    } else if (a.kind == LauncherAction::Kind::PlayArcade) {
      stop();
      arcade_.start((Arcade::Game)a.arcade, (uint32_t)plat_now_us());
      mode_ = Mode::Arcade;
      nativeDueUs_ = plat_now_us();
    }
    plat_sleep_us(2000);
    return;
  }

  switch (mode_) {
    case Mode::Gb: {
      constexpr uint32_t kGbFrameUs = 16742;  // 59.73 Hz
      if (now >= nativeDueUs_) {
        const uint64_t f0 = plat_now_us();
        // Behind schedule: emulate without presenting so the game keeps real speed.
        const bool present = f0 < nativeDueUs_ + kGbFrameUs;
        gb_.frame(in, present);
        nativeDueUs_ += kGbFrameUs;
        if (plat_now_us() > nativeDueUs_ + 6ull * kGbFrameUs) nativeDueUs_ = plat_now_us();
        // Autosave at most every 2 minutes. Many games use the save RAM as scratch memory
        // all the time, and a flash write pauses the game for ~0.6 s, so never save on every change.
        // Leaving the game (MENU) always saves.
        if (gb_.saveDirty()) {
          if (!gbDirtySinceUs_) gbDirtySinceUs_ = f0;
          else if (f0 - gbDirtySinceUs_ > 120000000ull) { saveGb(); gbDirtySinceUs_ = 0; }
        }
      }
      sleepUntil(nativeDueUs_);
      break;
    }
    case Mode::Nes: {
      constexpr uint32_t kNesFrameUs = 16639;  // 60.1 Hz
      if (now >= nativeDueUs_) {
        const uint64_t f0 = plat_now_us();
        nes_.frame(in);
        nativeDueUs_ += kNesFrameUs;
        if (plat_now_us() > nativeDueUs_ + 6ull * kNesFrameUs) nativeDueUs_ = plat_now_us();
        if (nes_.saveDirty()) {  // same policy as the Game Boy: at most every 2 minutes, and on exit
          if (!gbDirtySinceUs_) gbDirtySinceUs_ = f0;
          else if (f0 - gbDirtySinceUs_ > 120000000ull) { saveNes(); gbDirtySinceUs_ = 0; }
        }
      }
      sleepUntil(nativeDueUs_);
      break;
    }
    case Mode::Atari: {
      constexpr uint32_t kAtariFrameUs = 16688;  // NTSC 59.92 Hz
      if (now >= nativeDueUs_) {
        atari_.frame(in);
        nativeDueUs_ += kAtariFrameUs;
        if (plat_now_us() > nativeDueUs_ + 6ull * kAtariFrameUs) nativeDueUs_ = plat_now_us();
      }
      sleepUntil(nativeDueUs_);
      break;
    }
    case Mode::Gamepad: {
      const uint16_t held = input_.state().held;  // unmasked: BTN_MENU = SELECT+START
      pad_.update(held, g_padStick, now);
      const int pct = menuHold_ ? 1 + (int)((now - menuHoldUs_) * 100 / kMenuHoldUs) : 0;
      padView_.draw(held, pad_.hostReady(), g_padStick, pct > 100 ? 100 : pct);
      plat_sleep_us(500);  // ~1 ms button-to-report latency
      break;
    }
    case Mode::Arcade:
    case Mode::Neko:
      if (now >= nativeDueUs_) {
        // The loop polls faster than the 60 Hz game step, so hand over every edge seen since
        // the previous step, not just the ones from this poll.
        in.pressed = accPressed_;
        in.released = accReleased_;
        in.repeated = accRepeated_;
        accPressed_ = accReleased_ = accRepeated_ = 0;
        const uint64_t f0 = plat_now_us();
        if (mode_ == Mode::Arcade) {
          arcade_.frame(in);
        } else if (mode_ == Mode::Neko) {
          neko_.step(in);
          Canvas c{renderer_.fb(), Display::W, Display::H};
          neko_.draw(c);
          renderer_.presentDiff();
          if (f0 - nekoSavedUs_ > 60000000ull) { saveNeko(); nekoSavedUs_ = f0; }
        }
        nativeDueUs_ += kNativeFrameUs;
        if (plat_now_us() > nativeDueUs_ + 4ull * kNativeFrameUs) nativeDueUs_ = plat_now_us();
      }
      sleepUntil(nativeDueUs_);
      break;
    default:
      plat_sleep_us(1000);
  }
}
