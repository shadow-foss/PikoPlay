#include "emulation/nes_player.h"
#include <stdarg.h>
#include <string.h>
#include <pico.h>
#include "emulation/emu_pool.h"

#include "InfoNES.h"
#include "InfoNES_System.h"
#include "InfoNES_pAPU.h"

namespace {
NesPlayer* g_nes = nullptr;

// NES palette, from pico-infones' RGB555 table, as RGB565 spread out for blending: green moved
// to bits 21-26 (c | c << 16, masked) so r, g and b each have 5 spare bits above them and one
// 32-bit multiply scales all three at once.
constexpr uint32_t kSpread = 0x07E0F81Fu;
// Core 1's scaler, palette and tables live in SCRATCH_X (core 1's own 4 KB RAM bank) so they do
// not compete with core 0 for main RAM.
__scratch_x("nes") uint32_t kNesExp[64];
__scratch_x("nes") uint16_t g_colTab[NesPlayer::kW];
__scratch_x("nes") uint16_t g_rowTab[NesPlayer::kH];
const uint16_t kNes555[64] = {
  0x39ce, 0x1071, 0x0015, 0x2013, 0x440e, 0x5402, 0x5000, 0x3c20, 0x20a0, 0x0100, 0x0140, 0x00e2, 0x0ceb, 0x0000, 0x0000, 0x0000,
  0x5ef7, 0x01dd, 0x10fd, 0x401e, 0x5c17, 0x700b, 0x6ca0, 0x6521, 0x45c0, 0x0240, 0x02a0, 0x0247, 0x0211, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x1eff, 0x2e5f, 0x223f, 0x79ff, 0x7dd6, 0x7dcc, 0x7e67, 0x7ae7, 0x4342, 0x2769, 0x2ff3, 0x03bb, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x579f, 0x635f, 0x6b3f, 0x7f1f, 0x7f1b, 0x7ef6, 0x7f75, 0x7f94, 0x73f4, 0x57d7, 0x5bf9, 0x4ffe, 0x0000, 0x0000, 0x0000};
uint32_t g_sramCrc = 0;
}  // namespace

// InfoNES draws palette entries straight into the line: make them plain indices 0..63.
const WORD NesPalette[64] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};

int InfoNES_Menu() { return 0; }
int InfoNES_ReadRom(const char*) { return -1; }  // ROMs come from the flash slot (NesPlayer::start)
void InfoNES_ReleaseRom() { ROM = nullptr; VROM = nullptr; }
void InfoNES_LoadFrame() {}
void InfoNES_DebugPrint(const char*) {}
void InfoNES_MessageBox(const char*, ...) {}
void InfoNES_SoundInit() {}
int InfoNES_SoundOpen(int, int) { return 0; }
void InfoNES_SoundClose() {}
int InfoNES_GetSoundBufferSize() { return 0; }  // no speaker: never render sound
void InfoNES_SoundOutput(int, BYTE*, BYTE*, BYTE*, BYTE*, BYTE*) {}
void InfoNES_PadState(DWORD* pad1, DWORD* pad2, DWORD* system) {
  *pad1 = g_nes ? g_nes->pad() : 0;
  *pad2 = 0;
  *system = 0;
}
void InfoNES_PreDrawLine(int) { InfoNES_SetLineBuffer(g_nes->lineBuffer(), 256); }
void InfoNES_PostDrawLine(int line) { g_nes->postLine(line); }

const char* NesPlayer::start(const uint8_t* rom, uint32_t size) {
  running_ = false;
  if (size < 16 || memcmp(rom, "NES\x1A", 4)) return "NOT AN NES ROM";
  memcpy(&NesHeader, rom, sizeof NesHeader);
  const int mapper = (NesHeader.byInfo1 >> 4) | (NesHeader.byInfo2 & 0xF0);
  if (mapper == 235) return "UNSUPPORTED MAPPER";
  const uint8_t* p = rom + sizeof NesHeader;
  const uint32_t prg = NesHeader.byRomSize * 0x4000u, chr = NesHeader.byVRomSize * 0x2000u;
  const uint32_t trainer = (NesHeader.byInfo1 & 4) ? 512 : 0;
  if (sizeof NesHeader + trainer + prg + chr > size) return "ROM FILE TRUNCATED";

  for (int i = 0; i < 64; ++i) {
    const uint16_t c = kNes555[i];
    const uint32_t rgb = (((c >> 10) & 31u) << 11) | (((c >> 5) & 31u) << 6) | (c & 31u);
    kNesExp[i] = (rgb | rgb << 16) & kSpread;
  }
  // Clear the screen once (letterbox), then lend the framebuffers to the emulator.
  r_.clear(0);
  r_.present();
  // The framebuffer + shadow arena (151 KB) holds everything big:
  //   2 source frames 2 x 57,344 | CHR cache 32,768 | sprite RAM 256 | line hashes 3 x 896
  //   | 2 DMA batches 2 x 1,760
  constexpr uint32_t kFrame = kSrcW * kSrcLines;
  constexpr uint32_t kChr = 256 * 2 * 8 * 8;              // InfoNES' decoded pattern tables
  constexpr uint32_t kBatch = kBatchRows * kW * 2;
  constexpr uint32_t kNeed = 2 * kFrame + kChr + 256 + 3 * kSrcLines * 4 + 2 * kBatch;
  static_assert(kNeed <= Renderer::kArenaBytes, "NES arena");
  uint8_t* p8 = r_.arena();
  if (!p8) return "NO MEMORY";
  buf_[0] = p8;  p8 += kFrame;
  buf_[1] = p8;  p8 += kFrame;
  ChrBuf = p8;   p8 += kChr;
  SPRRAM = p8;   p8 += 256;
  hash_[0] = (uint32_t*)p8;
  hash_[1] = hash_[0] + kSrcLines;
  shownHash_ = hash_[1] + kSrcLines;
  p8 += 3 * kSrcLines * 4;
  colTab_ = g_colTab;
  rowTab_ = g_rowTab;
  batch_[0] = (uint16_t*)p8;
  batch_[1] = batch_[0] + kBatchRows * kW;
  RAM = g_emuPool;                       // 8 KB
  SRAM = g_emuPool + 0x2000;             // 8 KB battery RAM
  PPURAM = g_emuPool + 0x4000;           // 16 KB
  memset(g_emuPool, 0, sizeof g_emuPool);
  memset(buf_[0], 0, kFrame);
  memset(buf_[1], 0, kFrame);
  memset(hash_[0], 0, 3 * kSrcLines * 4);

  // Bilinear sample positions, pixel-centre aligned, in 1/32 source pixels. The last column/line
  // gets weight 0 for its (nonexistent) neighbour.
  for (int dx = 0; dx < kW; ++dx) {
    int pos = ((2 * dx + 1) * kSrcW * 32) / (2 * kW) - 16;
    if (pos < 0) pos = 0;
    if (pos >= (kSrcW - 1) * 32) pos = (kSrcW - 1) * 32;
    colTab_[dx] = (uint16_t)pos;
  }
  for (int dy = 0; dy < kH; ++dy) {
    int pos = ((2 * dy + 1) * kSrcLines * 32) / (2 * kH) - 16;
    if (pos < 0) pos = 0;
    if (pos >= (kSrcLines - 1) * 32) pos = (kSrcLines - 1) * 32;
    rowTab_[dy] = (uint16_t)pos;
  }

  if (trainer) { memcpy(&SRAM[0x1000], p, 512); p += 512; }
  ROM = (BYTE*)p;
  VROM = chr ? (BYTE*)(p + prg) : nullptr;


  g_nes = this;
  APU_Mute = 1;
  static bool inited = false;  // 6502 flag tables + scanline table, once per boot
  if (!inited) { InfoNES_Init(); inited = true; }
  if (InfoNES_Reset() < 0) { g_nes = nullptr; return "UNSUPPORTED MAPPER"; }
  SRAMwritten = false;
  g_sramCrc = 0;
  frames_ = shown_ = 0;
  draw_ = 0;
  ready_ = -1;
  firstFrame_ = true;
  running_ = true;
  return nullptr;
}

bool NesPlayer::hasBattery() const { return (NesHeader.byInfo1 & 2) != 0; }
const uint8_t* NesPlayer::saveData() const { return SRAM; }
bool NesPlayer::saveDirty() const { return SRAMwritten; }
void NesPlayer::clearSaveDirty() { SRAMwritten = false; }

void NesPlayer::loadSave(RomSource& src) {
  if (hasBattery()) src.read(0, SRAM, kSaveSize);
  SRAMwritten = false;
}

void NesPlayer::stop() {
  running_ = false;
  while (ready_ >= 0) {}
  r_.finishPixels();
  g_nes = nullptr;
  ChrBuf = SPRRAM = RAM = SRAM = PPURAM = nullptr;
  r_.invalidateDiff();
}

// One finished NES line (256 x uint16 palette indices) -> 256 bytes, plus its hash. Runs from RAM.
static uint32_t __not_in_flash_func(packLine)(const uint32_t* src, uint32_t* dst) {
  uint32_t h = 2166136261u;
  for (int i = 0; i < NesPlayer::kSrcW / 4; ++i) {
    const uint32_t p01 = src[0], p23 = src[1];
    src += 2;
    const uint32_t w = (p01 & 0x3Fu) | ((p01 >> 8) & 0x3F00u) | ((p23 & 0x3Fu) << 16) | ((p23 << 8) & 0x3F000000u);
    *dst++ = w;
    h = (h ^ w) * 16777619u;
  }
  return h;
}

void NesPlayer::postLine(int line) {
  const int sy = line - kSrcTop;
  if (sy < 0 || sy >= kSrcLines) return;
  hash_[draw_][sy] = packLine((const uint32_t*)lineBuffer(), (uint32_t*)(buf_[draw_] + sy * kSrcW));
}

void NesPlayer::frame(const InputState& in) {
  if (!running_) return;
  const uint16_t h = in.held;
  // InfoNES pad bits: A 1, B 2, SELECT 4, START 8, UP 16, DOWN 32, LEFT 64, RIGHT 128.
  pad_ = ((h & BTN_A) ? 1u : 0) | ((h & BTN_B) ? 2u : 0) | ((h & BTN_SELECT) ? 4u : 0) | ((h & BTN_START) ? 8u : 0) |
         ((h & BTN_UP) ? 16u : 0) | ((h & BTN_DOWN) ? 32u : 0) | ((h & BTN_LEFT) ? 64u : 0) | ((h & BTN_RIGHT) ? 128u : 0);
  InfoNES_Cycle();  // runs until the end of one frame
  ++frames_;
  if (ready_ < 0) {
    __sync_synchronize();
    ready_ = draw_;
    draw_ = draw_ == 0 ? 1 : 0;
  }
}

static inline uint32_t blend(uint32_t a, uint32_t b, uint32_t w) {  // w: 0..32 parts of b
  return ((a * (32 - w) + b * w) >> 5) & kSpread;
}

// Panel rows y0..y1-1 from the source frame, bilinear. In RAM: core 1 runs this while core 0
// keeps the flash cache busy with the emulator.
static void __attribute__((noinline)) __scratch_x("nes_code") scaleRows(const uint8_t* src, const uint16_t* cols, const uint16_t* rows, int y0, int y1,
                                           uint16_t* out) {
  constexpr int kW = NesPlayer::kW, kSrcW = NesPlayer::kSrcW;
  for (int y = y0; y < y1; ++y) {
    const uint32_t ry = rows[y], sy = ry >> 5, wy = ry & 31;
    const uint8_t* r0 = src + sy * kSrcW;
    const uint8_t* r1 = wy ? r0 + kSrcW : r0;
    for (int x = 0; x < kW; ++x) {
      const uint32_t cx = cols[x], sx = cx >> 5, wx = cx & 31;
      uint32_t c;
      if (wx) {
        const uint32_t top = blend(kNesExp[r0[sx]], kNesExp[r0[sx + 1]], wx);
        c = wy ? blend(top, blend(kNesExp[r1[sx]], kNesExp[r1[sx + 1]], wx), wy) : top;
      } else {
        c = wy ? blend(kNesExp[r0[sx]], kNesExp[r1[sx]], wy) : kNesExp[r0[sx]];
      }
      *out++ = (uint16_t)(c | c >> 16);
    }
  }
}

// Panel rows whose source lines changed are scaled in batches of up to kBatchRows and sent by DMA;
// the next batch is scaled while the previous one is on the wire.
void NesPlayer::presentService() {
  const int f = ready_;
  if (!running_ || f < 0) return;
  const uint8_t* src = buf_[f];
  const uint32_t* h = hash_[f];
  const bool all = firstFrame_;
  auto dirty = [&](int y) {
    if (all) return true;
    const uint32_t ry = rowTab_[y], sy = ry >> 5;
    return h[sy] != shownHash_[sy] || ((ry & 31) && h[sy + 1] != shownHash_[sy + 1]);
  };
  int y = 0, b = 0;
  while (y < kH) {
    if (!dirty(y)) { ++y; continue; }
    const int y0 = y++;
    while (y < kH && y - y0 < kBatchRows && dirty(y)) ++y;
    scaleRows(src, colTab_, rowTab_, y0, y, batch_[b]);
    r_.pushPixelsAsync(0, y0, kW, y - y0, batch_[b]);
    b ^= 1;
  }
  r_.finishPixels();
  memcpy(shownHash_, h, kSrcLines * 4);
  firstFrame_ = false;
  ++shown_;
  __sync_synchronize();
  ready_ = -1;
}
