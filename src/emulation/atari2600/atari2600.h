#pragma once
#include <stddef.h>
#include <stdint.h>
#include "emulation/atari2600/mos6502.h"

// Atari 2600 (NTSC): 6507 CPU, TIA video, RIOT timer and RAM, cartridge banking. No audio.
// The TIA keeps a per-line mask of which objects cover each pixel and rebuilds it only when a
// register changes. Output: 160x192 bytes per frame (colour >> 1), from line 40 after VSYNC.
namespace a2600 {

constexpr int kW = 160, kH = 192;
constexpr int kFirstLine = 40;  // VSYNC(3) + VBLANK(37)
extern const uint16_t kPalette565[128];

struct Input {
  bool up = false, down = false, left = false, right = false, fire = false, reset = false, select = false;
};

class Tia {
 public:
  // Object bits in the per-pixel mask.
  enum : uint8_t { kP0 = 1, kP1 = 2, kM0 = 4, kM1 = 8, kBL = 16, kPF = 32 };

  void write(uint8_t reg, uint8_t v);
  uint8_t read(uint8_t reg) const;
  void run(int clocks);  // advance colour clocks, drawing as it goes

  // State.
  uint16_t clock = 0, line = 0;  // clock 0..227 within the line; lines since VSYNC began
  bool vsync = false, vblank = false, wsync = false, frameDone = false, hmoveBlank = false;
  uint8_t colup0 = 0, colup1 = 0, colupf = 0, colubk = 0, ctrlpf = 0;
  uint8_t pf[3] = {}, nusiz[2] = {}, grp[2] = {}, grpOld[2] = {};
  bool refp[2] = {}, vdelp[2] = {}, enam[2] = {}, resmp[2] = {};
  bool enabl = false, enablOld = false, vdelbl = false;
  uint8_t posP[2] = {}, posM[2] = {}, posBL = 0, hmP[2] = {}, hmM[2] = {}, hmBL = 0;
  uint16_t coll = 0;  // 15 collision latches, see read()
  bool fire[2] = {};
  uint16_t linesThisFrame = 0;
  uint8_t* out = nullptr;  // kW x kH frame being drawn (the machine swaps it on each frame)

 private:
  uint8_t mask_[kW] = {};
  uint8_t dirty_ = 0xFF;  // objects whose bits in mask_ are stale
  uint8_t scratchRow_[kW] = {};
  void rebuild();
  void drawSpan(int x0, int x1);
  void endLine();
  void finishFrame();
  uint8_t playerPos() const { return clock < 68 ? 3 : (uint8_t)((clock - 68 + 5) % 160); }
  uint8_t missilePos() const { return clock < 68 ? 2 : (uint8_t)((clock - 68 + 4) % 160); }
};

class Riot {
 public:
  uint8_t ram[128] = {};
  uint8_t swcha = 0xFF, swchb = 0x3F | 0x08, ddra = 0, timer = 0xFF;
  uint16_t interval = 1024, count = 1024;
  bool underflow = false;
  void tick() { advance(1); }
  void advance(uint32_t n) {  // n CPU cycles
    while (n) {
      if (underflow) { timer = (uint8_t)(timer - n); return; }  // after underflow: -1 per cycle
      if (n < count) { count = (uint16_t)(count - n); return; }
      n -= count;
      count = interval;
      if (timer == 0) { underflow = true; timer = 0xFF; } else --timer;
    }
  }
  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t v);
};

enum class Cart : uint8_t { Rom2K, Rom4K, F8, FA, F6, F4 };

class Machine {
 public:
  // rom must stay valid while the machine runs. Returns nullptr or an error text.
  const char* load(const uint8_t* rom, uint32_t size);
  void setInput(const Input& in);
  void runFrame();  // until the game finishes a frame (VSYNC), or ~2 frames of cycles

  // Bus (used by the CPU). The TIA and RIOT run lazily: cycles are counted and they catch up
  // (sync) right before the CPU touches them, when WSYNC halts the CPU, and between frames.
  // Same results as stepping them every cycle, but pixels are drawn in long runs.
  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t v);
  inline void cycle() { ++pending_; }
  bool stalled() {
    if (tia.wsync) sync();
    return tia.wsync;
  }
  void sync() {
    if (!pending_) return;
    const uint32_t n = pending_;
    pending_ = 0;
    tia.run((int)(n * 3));
    riot.advance(n);
  }
  void skipWsync();  // RDY low: jump the CPU to the end of the scan line in one go

  Tia tia;
  Riot riot;
  Mos6502<Machine, true> cpu{*this};
  uint8_t bank = 0;
  Cart cart = Cart::Rom4K;

 private:
  const uint8_t* rom_ = nullptr;
  uint8_t ram256_[256] = {};  // FA cartridge RAM
  uint32_t pending_ = 0;      // CPU cycles the TIA/RIOT have not caught up with yet
  uint8_t bankCount() const;
  void hotspot(uint16_t a);
};

}  // namespace a2600
