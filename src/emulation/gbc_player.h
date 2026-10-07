#pragma once
#include <stddef.h>
#include <stdint.h>
#include "display/renderer.h"
#include "input/input.h"
#include "storage/rom_source.h"

// Game Boy Color player (also runs original Game Boy games), built on Walnut-CGB
// (lib/walnut_cgb, MIT). Core 0 emulates; core 1 (presentService) shows finished frames.
//
// Memory: while a game runs the launcher's framebuffer is idle, so the 50 KB emulator state
// lives inside it, and three 160x144 byte frames live inside the Renderer's shadow buffer. Each
// frame byte is an index into that frame's 64-colour palette snapshot (RGB565).
class GbcPlayer {
 public:
  static constexpr int kW = 160, kH = 144;
  static constexpr int kX = (Display::W - kW) / 2, kY = (Display::H - kH) / 2;
  // FIT mode: nearest-neighbour scale to the full panel height, same aspect ratio.
  static constexpr int kFitW = 195, kFitH = Display::H;
  static constexpr int kFitX = (Display::W - kFitW) / 2;
  static constexpr size_t kMaxSave = 32768;

  explicit GbcPlayer(Renderer& r);
  // Returns a short error for the launcher toast, or nullptr on success.
  const char* start(const uint8_t* rom, uint32_t size);
  void loadSave(RomSource& src);
  void stop();
  bool running() const { return running_; }
  bool colorMode() const;  // true: game runs as a Game Boy Color title

  void frame(const InputState& in, bool present);  // core 0: one frame
  void presentService();                           // core 1: show the latest finished frame
  uint32_t frames() const { return frames_; }
  uint32_t framesShown() const { return shown_; }
  const char* title() const { return title_; }

  // Save file = cartridge RAM, then (MBC3 games with a real-time clock) a 12-byte clock record:
  // "RTC1", the 5 clock registers (sec, min, hour, day low, day high/flags), 3 zero bytes.
  // The board has no clock chip: the in-game clock runs while playing and pauses when off.
  static constexpr size_t kRtcTail = 12;
  bool hasRtc() const;
  void rtcTail(uint8_t out[kRtcTail]) const;
  const uint8_t* saveData() const { return cartRam_; }
  size_t saveSize() const { return saveSize_; }
  bool saveDirty() const { return saveDirty_; }
  void clearSaveDirty() { saveDirty_ = false; }

  // C-callback entry points
  const uint8_t* rom() const { return rom_; }
  uint32_t romSize() const { return romSize_; }
  uint8_t cartRam(uint32_t a) const { return a < saveSize_ ? cartRam_[a] : 0xFF; }
  void cartRamWrite(uint32_t a, uint8_t v) {
    if (a < saveSize_ && cartRam_[a] != v) { cartRam_[a] = v; saveDirty_ = true; }
  }
  void drawLine(const uint8_t* pixels, int line);

 private:
  Renderer& r_;
  const uint8_t* rom_ = nullptr;
  uint32_t romSize_ = 0;
  uint8_t* cartRam_;  // = g_emuPool (shared with the NES player)
  size_t saveSize_ = 0;
  bool saveDirty_ = false;
  bool running_ = false;
  char title_[17] = {};
  uint32_t frames_ = 0;

  uint8_t* buf_[3] = {};
  uint16_t pal_[3][64] = {};           // palette snapshot per frame buffer
  int draw_ = 0;                       // buffer core 0 draws into
  volatile int ready_ = -1;            // buffer waiting for core 1 (-1 none)
  int onScreen_ = 2;                   // buffer mirroring what is on the panel
  volatile bool firstFrame_ = true;
  volatile uint32_t shown_ = 0;
  bool fit_ = true;
  uint8_t fitX0_[kW], fitX1_[kW];     // destination columns [x0, x1] for each source column
  uint8_t fitY0_[kH], fitY1_[kH];     // destination rows for each source row
  uint8_t fitSrc_[kFitW];             // source column for each destination column
  void presentFit(const uint8_t* src, uint8_t* shown, const uint16_t* pal, bool all);
};
