#include "emulation/gbc_player.h"
#include <new>
#include <string.h>

#define ENABLE_SOUND 0
#define ENABLE_LCD 1
#define WALNUT_FULL_GBC_SUPPORT 1
// Run the emulator's hot path from RAM on the handheld (flash is behind a 16 KB cache that the
// CPU core and the ROM reads fight over). See lib/walnut_cgb/PATCHES.md.
#define WALNUT_HOT __attribute__((section(".time_critical.walnut"), noinline))
#include "walnut_cgb.h"
#include "gfx/theme.h"
#include "emulation/emu_pool.h"



namespace {
struct gb_s* g_gb = nullptr;  // placed inside the Renderer framebuffer while a game runs
static_assert(sizeof(struct gb_s) <= sizeof(uint16_t) * Display::W * Display::H, "emulator state must fit in the framebuffer");

GbcPlayer* self(struct gb_s* gb) { return (GbcPlayer*)gb->direct.priv; }
uint8_t rd8(struct gb_s* gb, const uint_fast32_t a) {
  GbcPlayer* p = self(gb);
  return a < p->romSize() ? p->rom()[a] : 0xFF;
}
uint16_t rd16(struct gb_s* gb, const uint_fast32_t a) {
  GbcPlayer* p = self(gb);
  if (a + 1 >= p->romSize()) return (uint16_t)(rd8(gb, a) | (rd8(gb, a + 1) << 8));
  const uint8_t* q = p->rom() + a;
  return (uint16_t)(q[0] | (q[1] << 8));  // byte loads: the M0+ faults on unaligned 16-bit reads
}
uint32_t rd32(struct gb_s* gb, const uint_fast32_t a) {
  GbcPlayer* p = self(gb);
  if (a + 3 >= p->romSize()) return (uint32_t)rd16(gb, a) | ((uint32_t)rd16(gb, a + 2) << 16);
  const uint8_t* q = p->rom() + a;
  return (uint32_t)q[0] | ((uint32_t)q[1] << 8) | ((uint32_t)q[2] << 16) | ((uint32_t)q[3] << 24);
}
uint8_t ramRead(struct gb_s* gb, const uint_fast32_t a) { return self(gb)->cartRam((uint32_t)a); }
void ramWrite(struct gb_s* gb, const uint_fast32_t a, const uint8_t v) { self(gb)->cartRamWrite((uint32_t)a, v); }
void onError(struct gb_s*, const enum gb_error_e, const uint16_t) {}
void lcdLine(struct gb_s* gb, const uint8_t* px, const uint_fast8_t line) { self(gb)->drawLine(px, (int)line); }

}  // namespace

GbcPlayer::GbcPlayer(Renderer& r) : r_(r), cartRam_(g_emuPool) {}

bool GbcPlayer::colorMode() const { return g_gb && g_gb->cgb.cgbMode; }

const char* GbcPlayer::start(const uint8_t* rom, uint32_t size) {
  running_ = false;
  if (!rom || size < 0x150) return "NOT A GAME BOY ROM";
  rom_ = rom;
  romSize_ = size;
  memset(cartRam_, 0xFF, kMaxSave);
  // Letterbox once, then the framebuffer becomes the emulator's memory.
  r_.clear(rgb565(10, 10, 14));
  r_.present();
  g_gb = new (r_.fb()) gb_s();
  const enum gb_init_error_e e = gb_init(g_gb, rd8, rd16, rd32, ramRead, ramWrite, onError, this);
  switch (e) {
    case GB_INIT_NO_ERROR: break;
    case GB_INIT_CARTRIDGE_UNSUPPORTED: g_gb = nullptr; return "UNSUPPORTED CARTRIDGE";
    case GB_INIT_INVALID_CHECKSUM: g_gb = nullptr; return "BAD ROM CHECKSUM";
    default: g_gb = nullptr; return "CANNOT START ROM";
  }
  size_t ram = 0;
  if (gb_get_save_size_s(g_gb, &ram) != 0 || ram > kMaxSave) { g_gb = nullptr; return "SAVE RAM TOO LARGE"; }
  saveSize_ = ram;
  saveDirty_ = false;
  gb_init_lcd(g_gb, lcdLine);
  gb_get_rom_name(g_gb, title_);
  frames_ = shown_ = 0;
  uint8_t* sb = r_.shadowBytes();
  for (int i = 0; i < 3; ++i) buf_[i] = sb + i * kW * kH;
  memset(sb, 0xFF, 3 * kW * kH);
  fit_ = g_gbScale == 0;
  // Source pixel -> destination span. Every destination column/row maps to exactly one source.
  for (int i = 0; i < kW; ++i) { fitX0_[i] = 255; fitX1_[i] = 0; }
  for (int dx = 0; dx < kFitW; ++dx) {
    const int sx = dx * kW / kFitW;
    fitSrc_[dx] = (uint8_t)sx;
    if (fitX0_[sx] == 255) fitX0_[sx] = (uint8_t)dx;
    fitX1_[sx] = (uint8_t)dx;
  }
  for (int i = 0; i < kH; ++i) { fitY0_[i] = 255; fitY1_[i] = 0; }
  for (int dy = 0; dy < kFitH; ++dy) {
    const int sy = dy * kH / kFitH;
    if (fitY0_[sy] == 255) fitY0_[sy] = (uint8_t)dy;
    fitY1_[sy] = (uint8_t)dy;
  }
  draw_ = 0;
  onScreen_ = 2;
  ready_ = -1;
  firstFrame_ = true;
  running_ = true;
  return nullptr;
}

bool GbcPlayer::hasRtc() const { return g_gb && g_gb->mbc == 3; }

void GbcPlayer::rtcTail(uint8_t out[kRtcTail]) const {
  static_assert(sizeof(g_gb->rtc_real.bytes) == 5, "MBC3 clock has 5 registers");
  memset(out, 0, kRtcTail);
  memcpy(out, "RTC1", 4);
  memcpy(out + 4, g_gb->rtc_real.bytes, 5);
}

void GbcPlayer::loadSave(RomSource& src) {
  if (saveSize_) src.read(0, cartRam_, saveSize_);
  if (hasRtc() && src.size() >= saveSize_ + kRtcTail) {
    uint8_t t[kRtcTail];
    src.read((uint32_t)saveSize_, t, kRtcTail);
    if (!memcmp(t, "RTC1", 4)) memcpy(g_gb->rtc_real.bytes, t + 4, 5);
  }
  saveDirty_ = false;
}

void GbcPlayer::stop() {
  running_ = false;
  while (ready_ >= 0) {}  // core 1 finishes the frame it is pushing
  g_gb = nullptr;
  r_.invalidateDiff();    // framebuffer and shadow held emulator data: launcher redraws fully
}

void GbcPlayer::drawLine(const uint8_t* px, int line) {
  if (line < 0 || line >= kH) return;
  uint8_t* d = buf_[draw_] + line * kW;
  if (g_gb->cgb.cgbMode) {
    for (int x = 0; x < kW; ++x) d[x] = px[x] & 0x3F;  // index into the CGB palette table
  } else {
    for (int x = 0; x < kW; ++x) d[x] = (uint8_t)((((px[x] >> 4) & 3) << 2) | (px[x] & 3));
  }
}

void GbcPlayer::frame(const InputState& in, bool present) {
  if (!running_) return;
  const uint16_t h = in.held;
  uint8_t j = 0xFF;
  if (h & BTN_A) j &= ~JOYPAD_A;
  if (h & BTN_B) j &= ~JOYPAD_B;
  if (h & BTN_SELECT) j &= ~JOYPAD_SELECT;
  if (h & BTN_START) j &= ~JOYPAD_START;
  if (h & BTN_RIGHT) j &= ~JOYPAD_RIGHT;
  if (h & BTN_LEFT) j &= ~JOYPAD_LEFT;
  if (h & BTN_UP) j &= ~JOYPAD_UP;
  if (h & BTN_DOWN) j &= ~JOYPAD_DOWN;
  g_gb->direct.joypad = j;
  gb_run_frame(g_gb);  // the compact variant fits the 16 KB flash cache
  ++frames_;
  (void)present;  // core 1 draws: offer every finished frame
  if (ready_ < 0) {
    // Snapshot the palette this frame was drawn with.
    uint16_t* pal = pal_[draw_];
    if (g_gb->cgb.cgbMode) {
      memcpy(pal, g_gb->cgb.fixPalette, sizeof pal_[0]);
    } else {  // original Game Boy games: the palette chosen in Settings, 3 layers x 4 shades
      // (index = layer * 4 + shade, layer 0 OBJ0, 1 OBJ1, 2 BG as Walnut-CGB reports them)
      for (int layer = 0; layer < 3; ++layer)
        for (int s = 0; s < 4; ++s) {
          const uint8_t* c = kGbPalettes[g_gbPalette % kGbPaletteCount][s];
          pal[layer * 4 + s] = rgb565(c[0], c[1], c[2]);
        }
    }
    __sync_synchronize();
    ready_ = draw_;
    draw_ = draw_ == 0 ? 1 : 0;
  }
}

void GbcPlayer::presentService() {
  const int f = ready_;
  if (!running_ || f < 0) return;
  static uint16_t strip[kW];
  const uint8_t* src = buf_[f];
  uint8_t* shown = buf_[onScreen_];
  const uint16_t* pal = pal_[f];
  // A palette change repaints everything (same indices, new colours).
  static uint16_t lastPal[64];
  const bool all = firstFrame_ || memcmp(lastPal, pal, sizeof lastPal);
  if (all) memcpy(lastPal, pal, sizeof lastPal);
  if (fit_) {
    presentFit(src, shown, pal, all);
    firstFrame_ = false;
    ++shown_;
    __sync_synchronize();
    ready_ = -1;
    return;
  }
  for (int y = 0; y < kH; ++y) {
    const uint8_t* a = src + y * kW;
    uint8_t* b = shown + y * kW;
    if (!all && !memcmp(a, b, kW)) continue;
    int x0 = 0, x1 = kW - 1;
    if (!all) {
      while (a[x0] == b[x0]) ++x0;
      while (a[x1] == b[x1]) --x1;
    }
    const int w = x1 - x0 + 1;
    for (int x = 0; x < w; ++x) strip[x] = pal[a[x0 + x] & 63];
    r_.pushPixels(kX + x0, kY + y, w, 1, strip);
    memcpy(b + x0, a + x0, w);
  }
  firstFrame_ = false;
  ++shown_;
  __sync_synchronize();
  ready_ = -1;
}

// FIT: each changed source line is pushed as its 1-2 destination rows, only over the changed
// columns (widened to whole destination pixels).
void GbcPlayer::presentFit(const uint8_t* src, uint8_t* shown, const uint16_t* pal, bool all) {
  static uint16_t strip[kFitW * 2];
  for (int y = 0; y < kH; ++y) {
    const uint8_t* a = src + y * kW;
    uint8_t* b = shown + y * kW;
    if (!all && !memcmp(a, b, kW)) continue;
    int x0 = 0, x1 = kW - 1;
    if (!all) {
      while (a[x0] == b[x0]) ++x0;
      while (a[x1] == b[x1]) --x1;
    }
    const int dx0 = fitX0_[x0], dx1 = fitX1_[x1], w = dx1 - dx0 + 1;
    const int dy0 = fitY0_[y], rows = fitY1_[y] - dy0 + 1;
    for (int dx = dx0; dx <= dx1; ++dx) strip[dx - dx0] = pal[a[fitSrc_[dx]] & 63];
    if (rows == 2) memcpy(strip + w, strip, (size_t)w * 2);
    r_.pushPixels(kFitX + dx0, dy0, w, rows, strip);
    memcpy(b + x0, a + x0, x1 - x0 + 1);
  }
}
