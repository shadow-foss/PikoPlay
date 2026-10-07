#include "ui/launcher.h"
#include <stdio.h>
#include <string.h>
#include "gfx/canvas.h"
#include "gfx/theme.h"
#include "native/arcade.h"
#include "platform.h"

namespace {
using System = LauncherUI::System;

struct SysInfo {
  const char* name;  // big carousel name (scale 3: at most 10 characters)
  const char* full;  // sub line under it
  const char* dir;   // ROM directory (emulator systems)
};
const SysInfo kSys[LauncherUI::kSystemCount] = {
  {"ARCADE", "BUILT-IN GAMES", nullptr},
  {"NEKO", "YOUR CAT: FEED, PET, PLAY", nullptr},
  {"ATARI 2600", "1977 HOME CONSOLE", "/roms/atari"},
  {"NES", "8-BIT HOME CONSOLE", "/roms/nes"},
  {"GAME BOY", "8-BIT HANDHELD", "/roms/gb"},
  {"GB COLOR", "8-BIT COLOUR HANDHELD", "/roms/gbc"},
  {"GAMEPAD", "BE A USB CONTROLLER FOR A PC", nullptr},
  {"SETTINGS", "THEME, SCREEN, CONTROLS, INFO", nullptr},
};
// 16x16 pixel icons, drawn at 3x on the carousel card.
const char* const kIcon[LauncherUI::kSystemCount][16] = {
  {"................", "......XXXX......", ".....XXXXXX.....", ".....XXXXXX.....", "......XXXX......", ".......XX.......",
   ".......XX.......", ".......XX.......", "..XXXXXXXXXXXX..", ".XXXXXXXXXXXXXX.", ".XX..XXXXXX..XX.", ".XX..XXXXXX..XX.",
   ".XXXXXXXXXXXXXX.", ".XXXXXXXXXXXXXX.", "................", "................"},
  {"................", ".XX..........XX.", ".XXX........XXX.", ".XXXX......XXXX.", ".XXXXXXXXXXXXXX.", ".XXXXXXXXXXXXXX.",
   ".XXX..XXXX..XXX.", ".XXX..XXXX..XXX.", ".XXXXXXXXXXXXXX.", ".XXXXXX..XXXXXX.", ".XXXXXXXXXXXXXX.", "..XXXXXXXXXXXX..",
   "...XXXXXXXXXX...", "................", "................", "................"},
  {"................", "................", "................", "...X..X..X..X...", "...X..X..X..X...", ".XXXXXXXXXXXXXX.",
   ".X............X.", ".X.XXXXXXXXXX.X.", ".X............X.", ".X.XXXXXXXXXX.X.", ".X............X.", ".XXXXXXXXXXXXXX.",
   ".XXXXXXXXXXXXXX.", "................", "................", "................"},
  {"................", "................", "................", "................", "................", "XXXXXXXXXXXXXXXX",
   "XXX.XXXXXXXXXXXX", "XX...XXXXX..X..X", "XXX.XX..XX..X..X", "XXXXXXXXXXXXXXXX", "XXXXXXXXXXXXXXXX", "................",
   "................", "................", "................", "................"},
  {"...XXXXXXXXXX...", "...XXXXXXXXXX...", "...X........X...", "...X........X...", "...X........X...", "...X........X...",
   "...X........X...", "...XXXXXXXXXX...", "...XXXXXXXXXX...", "...XX.XXXXX.X...", "...X...XXX.XX...", "...XX.XXXXXXX...",
   "...XXXXXXXXXX...", "...XXXX..XXXX...", "...XXXXXXXXXX...", "................"},
  {"...XXXXXXXXXX...", "...XXXXXXXXXX...", "...X........X...", "...X.XXXXXX.X...", "...X........X...", "...X.XXXXXX.X...",
   "...X........X...", "...XXXXXXXXXX...", "...XXXXXXXXXX...", "...XX.XXXXX.X...", "...X...XXX.XX...", "...XX.XXXXXXX...",
   "...XXXXXXXXXX...", "...XXXX..XXXX...", "...XXXXXXXXXX...", "................"},
  {"................", "................", "................", "...XXXXXXXXXX...", ".XXXXXXXXXXXXXX.", "XXX.XXXXXXXX.XXX",
   "XX...XXXXXX.X.XX", "XXX.XXXXXXXX.XXX", "XXXXXXXXXXXXXXXX", "XXXXX......XXXXX", "XXXX........XXXX", ".XX..........XX.",
   "................", "................", "................", "................"},
  {"................", ".......XX.......", "...XX..XX..XX...", "...XXXXXXXXXX...", "....XXXXXXXX....", "...XXXX..XXXX...",
   ".XXXXX....XXXXX.", ".XXXX......XXXX.", ".XXXX......XXXX.", ".XXXXX....XXXXX.", "...XXXX..XXXX...", "....XXXXXXXX....",
   "...XXXXXXXXXX...", "...XX..XX..XX...", ".......XX.......", "................"},
};

constexpr int kHeaderH = 18;
constexpr int kRowY = 21, kRowH = 13;
constexpr int kDetailY = 140, kFooterY = 158;
constexpr uint64_t kToastUs = 2200000;
constexpr uint64_t kConfirmUs = 3000000;

inline int emuIndex(System s) { return (int)s - (int)System::Atari; }  // Atari..Gbc -> 0..3
inline bool isEmu(System s) { return s >= System::Atari && s <= System::Gbc; }

void icon(Canvas& c, int x, int y, System s, uint16_t col, int scale) {
  const char* const* rows = kIcon[(int)s];
  for (int r = 0; r < 16; ++r)
    for (int k = 0; k < 16 && rows[r][k]; ++k)
      if (rows[r][k] == 'X') c.fill(x + k * scale, y + r * scale, scale, scale, col);
}

// A key cap: the key name inverted, then what it does. Returns the x after it.
int keycap(Canvas& c, int x, int y, const char* key, const char* what, const Theme& t) {
  const int kw = Canvas::textWidth(key) + 6;
  c.fill(x, y, kw, 11, t.text);
  c.text(x + 3, y + 2, key, t.bg);
  c.text(x + kw + 4, y + 2, what, t.text);
  return x + kw + 4 + Canvas::textWidth(what) + 12;
}

// Fixed-point megabytes, one decimal: 1234567 -> "1.2".
void mb(char* out, size_t n, uint32_t bytes) {
  const uint32_t tenths = (uint32_t)((bytes * 10ull + 524288) / 1048576);
  snprintf(out, n, "%lu.%lu", (unsigned long)(tenths / 10), (unsigned long)(tenths % 10));
}
}  // namespace

int LauncherUI::theme() const { return g_themeIndex % kThemeCount; }

void LauncherUI::open() {
  page_ = Page::Home;
  active_ = true;
  dirty_ = true;
  clearArmedUntilUs_ = 0;
  r_.invalidateDiff();  // a game or emulator drew directly on the panel
}

void LauncherUI::notify(const char* msg, uint64_t nowUs) {
  snprintf(toastText_, sizeof toastText_, "%s", msg);
  toastUntilUs_ = nowUs + kToastUs;
  dirty_ = true;
}


bool LauncherUI::hasCore(System s) const { return !isEmu(s) || ((coreMask_ >> emuIndex(s)) & 1); }
bool LauncherUI::isList(System s) const { return s == System::Arcade || (isEmu(s) && hasCore(s)); }
int LauncherUI::romCountOf(System s) const {
  return s == System::Arcade ? nGames_ : isEmu(s) ? romCount_[emuIndex(s)] : 0;
}

void LauncherUI::countRoms(Storage& storage) {
  countGen_ = storage.generation();
  for (int i = 0; i < 4; ++i) romCount_[i] = storage.countRoms(kSys[(int)System::Atari + i].dir);
  if (gamesGen_ != countGen_) {
    gamesGen_ = countGen_;
    scanGames();
    if (sel_[1] >= nGames_) sel_[1] = top_[1] = 0;
  }
  rebuildCarousel();
}

void LauncherUI::rebuildCarousel() {
  const System keep = nCarousel_ ? carousel_[home_] : System::Arcade;
  nCarousel_ = 0;
  for (int i = 0; i < kSystemCount; ++i) {
    const System s = (System)i;
    if (g_hideEmpty && isEmu(s) && romCount_[emuIndex(s)] == 0) continue;
    carousel_[nCarousel_++] = s;
  }
  home_ = 0;
  for (int i = 0; i < nCarousel_; ++i) if (carousel_[i] <= keep) home_ = i;  // same or the one before it
}

void LauncherUI::scanGames() {
  nGames_ = 0;
  for (int i = 0; i < Arcade::kGameCount; ++i) {
    Game& a = games_[nGames_++];
    memset(&a, 0, sizeof a);
    snprintf(a.title, sizeof a.title, "%s", Arcade::name((Arcade::Game)i));
    snprintf(a.detail, sizeof a.detail, "%s", Arcade::blurb((Arcade::Game)i));
    a.arcade = (int8_t)i;
  }
}

void LauncherUI::scanRoms(Storage& storage) {
  nRoms_ = 0;
  storage.forEachFile(kSys[(int)listSys_].dir, [](const char* name, uint32_t size, bool, void* v) {
    LauncherUI* ui = (LauncherUI*)v;
    if (!Storage::isRomFileName(name) || ui->nRoms_ >= kMaxRoms) return true;
    for (int i = 0; i < ui->nRoms_; ++i) if (!strcmp(ui->roms_[i].name, name)) return true;  // SD + flash copy
    if (strlen(name) >= sizeof ui->roms_[0].name) return true;
    strcpy(ui->roms_[ui->nRoms_].name, name);
    ui->roms_[ui->nRoms_].size = size;
    ++ui->nRoms_;
    return true;
  }, this);
  for (int i = 1; i < nRoms_; ++i) {  // alphabetical
    Rom t = roms_[i];
    int j = i - 1;
    while (j >= 0 && strcasecmp(roms_[j].name, t.name) > 0) { roms_[j + 1] = roms_[j]; --j; }
    roms_[j + 1] = t;
  }
}


LauncherUI::Row LauncherUI::gameRow(int i) const {
  const Game& g = games_[i];
  char* tag = scratch_[i % 3];
  snprintf(tag, sizeof scratch_[0], "HI %d", best_[g.arcade]);
  return {g.title, tag, false, false};
}

LauncherUI::Row LauncherUI::romRow(int i) const {
  char* title = scratch_[0];
  char* tag = scratch_[2];
  const char* dot = strrchr(roms_[i].name, '.');
  const int stem = dot ? (int)(dot - roms_[i].name) : (int)strlen(roms_[i].name);
  snprintf(title, sizeof scratch_[0], "%.*s", stem < 22 ? stem : 22, roms_[i].name);
  snprintf(tag, sizeof scratch_[0], "%luK", (unsigned long)((roms_[i].size + 1023) / 1024));
  return {title, tag, false, false};
}

LauncherUI::Row LauncherUI::settingRow(int i) const {
  switch (i) {
    case kSetTheme: return {"THEME", kThemes[g_themeIndex % kThemeCount].name, false, true};
    case kSetGbScreen: return {"GB SCREEN", g_gbScale ? "SHARP" : "FIT", false, true};
    case kSetGbPalette: return {"GB PALETTE", kGbPaletteNames[g_gbPalette % kGbPaletteCount], false, true};
    case kSetMenuSpeed: return {"MENU SPEED", kMenuSpeedNames[g_menuSpeed % kMenuSpeedCount], false, true};
    case kSetEmpty: return {"EMPTY SYSTEMS", g_hideEmpty ? "HIDE" : "SHOW", false, true};
    case kSetPadDpad: return {"PAD D-PAD", g_padStick ? "STICK" : "HAT", false, true};
    case kSetScores: return {"HIGH SCORES", clearArmedUntilUs_ ? "SURE?" : "CLEAR", false, false};
    case kInfoStorage: {
      char* v = scratch_[0];
      char a[8], b[8];
      mb(a, sizeof a, flashUsed_);
      mb(b, sizeof b, flashTotal_);
      if (!flashOk_ && !sdOk_) snprintf(v, sizeof scratch_[0], "OFFLINE");
      else snprintf(v, sizeof scratch_[0], "%s%s/%sM", sdOk_ ? "SD " : "", a, b);
      return {"STORAGE", v, !flashOk_ && !sdOk_, false};
    }
    case kInfoBrightness:
      return {"BRIGHTNESS", "FIXED", true, false};
    case kInfoBattery: return {"BATTERY", "N/A", true, false};
    default: return {"FIRMWARE", "0.5", false, false};
  }
}

const char* LauncherUI::detailLine() const {
  switch (page_) {
    case Page::Games:
      if (!nGames_) return "";
      return games_[sel_[1]].detail;
    case Page::Roms: {
      if (!nRoms_) return "";
      char* d = scratch_[1];
      snprintf(d, sizeof scratch_[1], "%.36s", roms_[sel_[2]].name);
      return d;
    }
    case Page::Settings:
      switch (sel_[3]) {
        case kSetTheme: return "COLOURS FOR MENUS AND GAMES";
        case kSetGbScreen: return g_gbScale ? "1:1 PIXELS, 160X144, BORDERED" : "SCALED TO THE FULL SCREEN HEIGHT";
        case kSetGbPalette: return "SHADES FOR ORIGINAL GB GAMES";
        case kSetMenuSpeed: return "D-PAD REPEAT WHILE HELD";
        case kSetEmpty: return "SYSTEMS WITHOUT ROMS ON HOME";
        case kSetPadDpad: return g_padStick ? "GAMEPAD MODE: D-PAD = LEFT STICK" : "GAMEPAD MODE: D-PAD = HAT SWITCH";
        case kSetScores: return clearArmedUntilUs_ ? "A AGAIN TO ERASE ALL BESTS" : "RESET ARCADE BESTS (A TWICE)";
        case kInfoStorage: return sdOk_ ? "SD CARD + FLASH, MB USED/TOTAL" : "INTERNAL FLASH, MB USED/TOTAL";
        case kInfoBrightness: return "BACKLIGHT WIRED TO 3.3V";
        case kInfoBattery: return "NO VOLTAGE SENSOR ON THIS PCB";
        default: return "PIKOPLAY";
      }
    default: return "";
  }
}


void LauncherUI::drawHeader(const char* title, int index, int count) {
  Canvas c{r_.fb(), Display::W, Display::H};
  const Theme& t = ::theme();
  c.fill(0, 0, Display::W, kHeaderH, t.accent);
  c.text(8, 6, title, t.ink);
  if (count > 0) {
    char b[12];
    snprintf(b, sizeof b, "%02d/%02d", index + 1, count);
    c.textRight(Display::W - 8, 6, b, t.ink);
  }
}

// Home: one big card for the selected system (hard offset shadow in the accent colour),
// the neighbours named under it. Everything that changes on a move is inside the card.
void LauncherUI::drawHome() {
  Canvas c{r_.fb(), Display::W, Display::H};
  const Theme& t = ::theme();
  const System s = carousel_[home_];
  c.text(8, 6, "PIKOPLAY", t.text);
  char b[12];
  snprintf(b, sizeof b, "%02d/%02d", home_ + 1, nCarousel_);
  c.textRight(Display::W - 8, 6, b, t.muted);
  c.fill(0, kHeaderH - 1, Display::W, 2, t.text);

  const int x = 8, y = 26, w = 200, h = 100;
  c.fill(x + 6, y + 6, w, h, t.accent);  // hard shadow
  c.fill(x, y, w, h, t.text);            // 2 px border
  c.fill(x + 2, y + 2, w - 4, h - 4, t.surface);

  // tag: how much is in there
  char tag[20];
  if (!hasCore(s)) snprintf(tag, sizeof tag, "NO CORE");
  else if (s == System::Neko) snprintf(tag, sizeof tag, "VIRTUAL PET");
  else if (s == System::Settings) snprintf(tag, sizeof tag, "SYSTEM");
  else if (s == System::Gamepad) snprintf(tag, sizeof tag, "USB HID");
  else { const int n = romCountOf(s); snprintf(tag, sizeof tag, "%d GAME%s", n, n == 1 ? "" : "S"); }
  const int tw = Canvas::textWidth(tag) + 8;
  const bool bad = !hasCore(s) || (isEmu(s) && romCountOf(s) == 0);
  c.fill(x + 8, y + 8, tw, 13, bad ? t.line : t.accent);
  c.text(x + 12, y + 11, tag, bad ? t.text : t.ink);

  icon(c, x + w - 12 - 48, y + 8, s, t.text, 3);
  c.text(x + 8, y + 62, kSys[(int)s].name, t.text, 3);
  c.text(x + 8, y + 88, kSys[(int)s].full, t.muted);

  // neighbours, RetroPie style
  if (nCarousel_ > 1) {
    const System prev = carousel_[(home_ + nCarousel_ - 1) % nCarousel_];
    const System next = carousel_[(home_ + 1) % nCarousel_];
    char l[16], r[16];
    snprintf(l, sizeof l, "< %s", kSys[(int)prev].name);
    snprintf(r, sizeof r, "%s >", kSys[(int)next].name);
    c.text(8, kDetailY + 4, l, t.muted);
    c.textRight(Display::W - 8, kDetailY + 4, r, t.muted);
  }
}

void LauncherUI::drawList(int count, Row (LauncherUI::*rowFn)(int) const) {
  Canvas c{r_.fb(), Display::W, Display::H};
  const Theme& t = ::theme();
  const int p = (int)page_;
  for (int k = 0; k < kVisibleRows && top_[p] + k < count; ++k) {
    const int i = top_[p] + k;
    const Row r = (this->*rowFn)(i);
    const int y = kRowY + k * kRowH;
    const bool sel = i == sel_[p];
    if (sel) c.fill(0, y, Display::W, kRowH, t.text);  // the selection is an inverted bar
    char idx[4];
    snprintf(idx, sizeof idx, "%02d", (i + 1) % 100);
    c.text(8, y + 3, idx, sel ? t.bg : t.muted);
    c.textClip(26, y + 3, r.title, sel ? t.bg : r.dim ? t.muted : t.text, 22);
    if (sel && r.knob) {
      char v[24];
      snprintf(v, sizeof v, "< %s >", r.tag);
      c.textRight(Display::W - 8, y + 3, v, t.bg);
    } else {
      c.textRight(Display::W - 8, y + 3, r.tag, sel ? t.bg : r.dim ? accent::warn : t.muted);
    }
  }
  if (count == 0) {
    c.textCenter(Display::W / 2, 54, "NOTHING HERE", t.text, 2);
    char where[40];
    snprintf(where, sizeof where, "PUT ROMS IN %s", kSys[(int)listSys_].dir ? kSys[(int)listSys_].dir : "/");
    c.textCenter(Display::W / 2, 82, where, t.muted);
    c.textCenter(Display::W / 2, 94, "OVER USB WITH PIKOLINK", t.muted);
  }
  c.fill(0, kDetailY, Display::W, 2, t.text);
  c.textClip(8, kDetailY + 6, detailLine(), t.muted, 34);
}

void LauncherUI::drawFooter() {
  Canvas c{r_.fb(), Display::W, Display::H};
  const Theme& t = ::theme();
  c.fill(0, kFooterY, Display::W, 2, t.text);
  if (toastUntilUs_) {  // a notice replaces the key hints, inverted in the warning colour
    c.fill(0, kFooterY + 2, Display::W, Display::H - kFooterY - 2, accent::warn);
    c.textCenter(Display::W / 2, kFooterY + 6, toastText_, rgb565(0, 0, 0));
    return;
  }
  const int y = kFooterY + 5;
  int x = 8;
  switch (page_) {
    case Page::Home:
      x = keycap(c, x, y, "A", "OPEN", t);
      keycap(c, x, y, "<>", "SYSTEM", t);
      break;
    case Page::Settings:
      x = keycap(c, x, y, "A", sel_[3] == kSetScores ? "CLEAR" : "CHANGE", t);
      x = keycap(c, x, y, "<>", "VALUE", t);
      keycap(c, x, y, "B", "BACK", t);
      break;
    default:
      if (page_ == Page::Games ? nGames_ : nRoms_) x = keycap(c, x, y, "A", "PLAY", t);
      x = keycap(c, x, y, "B", "BACK", t);
      keycap(c, x, y, "<>", "SYSTEM", t);
      break;
  }
}

void LauncherUI::render() {
  dirty_ = false;
  Canvas c{r_.fb(), Display::W, Display::H};
  const Theme& t = ::theme();
  c.fill(0, 0, Display::W, Display::H, t.bg);
  switch (page_) {
    case Page::Home: drawHome(); break;
    case Page::Games:
      drawHeader(kSys[(int)System::Arcade].name, sel_[1], nGames_);
      drawList(nGames_, &LauncherUI::gameRow);
      break;
    case Page::Roms:
      drawHeader(kSys[(int)listSys_].name, sel_[2], nRoms_);
      drawList(nRoms_, &LauncherUI::romRow);
      break;
    case Page::Settings:
      drawHeader("SETTINGS", sel_[3], kSettingCount);
      drawList(kSettingCount, &LauncherUI::settingRow);
      break;
  }
  drawFooter();
  lastSent_ = r_.presentDiff();
}

void LauncherUI::go(Page p, Storage& storage) {
  page_ = p;
  toastUntilUs_ = 0;
  toastText_[0] = 0;
  clearArmedUntilUs_ = 0;
  sdOk_ = storage.sdPresent();
  flashOk_ = storage.flashPresent();
  if (p == Page::Games && gamesGen_ != storage.generation()) {
    gamesGen_ = storage.generation();
    scanGames();
    if (sel_[1] >= nGames_) sel_[1] = 0;
    top_[1] = 0;
    if (sel_[1] >= kVisibleRows) top_[1] = sel_[1] - kVisibleRows + 1;
  } else if (p == Page::Roms) {
    scanRoms(storage);
    sel_[2] = top_[2] = 0;
  } else if (p == Page::Settings) {
    flashUsed_ = flashTotal_ = 0;
    if (flashOk_) storage.flashInfo(flashTotal_, flashUsed_);
  } else if (p == Page::Home && countGen_ != storage.generation()) {
    countRoms(storage);
  }
  dirty_ = true;
}

// Open what a carousel entry stands for (also used by LEFT/RIGHT inside a game list).
void LauncherUI::enter(System s, Storage& storage, uint64_t nowUs) {
  if (!hasCore(s)) {
    char m[40];
    snprintf(m, sizeof m, "NO %s CORE", kSys[(int)s].name);
    notify(m, nowUs);
    return;
  }
  listSys_ = s;
  if (s == System::Arcade) go(Page::Games, storage);
  else if (s == System::Settings) go(Page::Settings, storage);
  else go(Page::Roms, storage);
}

void LauncherUI::move(int dir, int count) {
  const int p = (int)page_;
  if (count <= 0) return;
  sel_[p] = (sel_[p] + dir + count) % count;
  if (sel_[p] < top_[p]) top_[p] = sel_[p];
  if (sel_[p] >= top_[p] + kVisibleRows) top_[p] = sel_[p] - kVisibleRows + 1;
  dirty_ = true;
}

// LEFT/RIGHT (dir = -1/+1) or A (dir = 0) on the selected settings row.
void LauncherUI::change(int dir, uint64_t nowUs, LauncherAction& act) {
  const int step = dir < 0 ? -1 : 1;
  auto cycle = [&](uint8_t& v, int n) { v = (uint8_t)((v + step + n) % n); dirty_ = true; };
  switch (sel_[3]) {
    case kSetTheme: cycle(g_themeIndex, kThemeCount); break;
    case kSetGbScreen: cycle(g_gbScale, 2); break;
    case kSetGbPalette: cycle(g_gbPalette, kGbPaletteCount); break;
    case kSetMenuSpeed: cycle(g_menuSpeed, kMenuSpeedCount); break;
    case kSetEmpty: cycle(g_hideEmpty, 2); rebuildCarousel(); break;
    case kSetPadDpad: cycle(g_padStick, 2); break;
    case kSetScores:
      if (dir != 0) break;
      if (clearArmedUntilUs_ && nowUs < clearArmedUntilUs_) {
        clearArmedUntilUs_ = 0;
        act.kind = LauncherAction::Kind::ClearScores;
        for (int& b : best_) b = 0;
        notify("HIGH SCORES CLEARED", nowUs);
      } else {
        clearArmedUntilUs_ = nowUs + kConfirmUs;
        dirty_ = true;
      }
      break;
    case kInfoBrightness: if (dir == 0) notify("NO BACKLIGHT CONTROL ON THIS BOARD", nowUs); break;
    case kInfoBattery: if (dir == 0) notify("NO BATTERY SENSOR FITTED", nowUs); break;
    default: if (dir == 0) notify("READ-ONLY", nowUs); break;
  }
}

LauncherAction LauncherUI::frame(const InputState& in, uint64_t nowUs, Storage& storage) {
  LauncherAction act;
  if (toastUntilUs_ && nowUs >= toastUntilUs_) {
    toastUntilUs_ = 0;
    toastText_[0] = 0;
    dirty_ = true;
  }
  if (clearArmedUntilUs_ && nowUs >= clearArmedUntilUs_) { clearArmedUntilUs_ = 0; dirty_ = true; }
  if (!sdOk_ && !flashOk_) { sdOk_ = storage.sdPresent(); flashOk_ = storage.flashPresent(); }
  if (page_ == Page::Home && countGen_ != storage.generation()) { countRoms(storage); dirty_ = true; }

  const uint16_t nav = in.repeated;
  const bool back = in.pressed & BTN_B;
  const bool ok = in.pressed & BTN_A;  // only A confirms; START is left for games
  const int vdir = (nav & BTN_DOWN) ? 1 : (nav & BTN_UP) ? -1 : 0;
  const int hdir = (nav & BTN_RIGHT) ? 1 : (nav & BTN_LEFT) ? -1 : 0;

  switch (page_) {
    case Page::Home: {
      if (hdir && nCarousel_) { home_ = (home_ + hdir + nCarousel_) % nCarousel_; dirty_ = true; }  // horizontal only
      if (ok && nCarousel_) {
        const System s = carousel_[home_];
        if (s == System::Neko || s == System::Gamepad) {
          act.kind = s == System::Neko ? LauncherAction::Kind::PlayNeko : LauncherAction::Kind::Gamepad;
          active_ = false;
        } else {
          enter(s, storage, nowUs);
        }
      }
      break;
    }
    case Page::Games:
    case Page::Roms: {
      const int p = (int)page_;
      const int count = page_ == Page::Games ? nGames_ : nRoms_;
      if (back) { go(Page::Home, storage); break; }
      if (hdir) {  // jump to the neighbouring system that has a game list
        for (int k = 1; k < nCarousel_; ++k) {
          const int i = ((home_ + hdir * k) % nCarousel_ + nCarousel_) % nCarousel_;
          if (isList(carousel_[i])) { home_ = i; enter(carousel_[i], storage, nowUs); break; }
        }
        break;
      }
      if (vdir) move(vdir, count);
      if (ok && page_ == Page::Games && nGames_) {
        act.kind = LauncherAction::Kind::PlayArcade;
        act.arcade = (uint8_t)games_[sel_[p]].arcade;
        active_ = false;
      } else if (ok && page_ == Page::Roms && nRoms_) {
        snprintf(act.path, sizeof act.path, "%s/%s", kSys[(int)listSys_].dir, roms_[sel_[p]].name);
        act.kind = LauncherAction::Kind::PlayRom;
        active_ = false;
      }
      break;
    }
    case Page::Settings:
      if (back) { go(Page::Home, storage); break; }
      if (vdir) { move(vdir, kSettingCount); clearArmedUntilUs_ = 0; }
      if (hdir) change(hdir, nowUs, act);
      else if (ok) change(0, nowUs, act);
      break;
  }
  if (active_ && dirty_) render();
  return act;
}
