#pragma once
#include <stdint.h>
#include "display/renderer.h"
#include "emulation/atari2600/atari2600.h"
#include "input/input.h"
#include "storage/rom_source.h"

// Atari 2600 player. Core 0 runs one frame per call; core 1 (presentService) shows it, 160x192
// cropped to 176 lines and widened to 220 columns. The ROM (at most 32 KB) is copied into RAM.
// Controls: D-pad = joystick, A or B = fire, START = RESET, SELECT = SELECT.
class AtariPlayer {
 public:
  static constexpr int kW = Display::W, kH = Display::H;
  static constexpr int kCropTop = (a2600::kH - Display::H) / 2;  // 8

  explicit AtariPlayer(Renderer& r) : r_(r) {}
  const char* start(RomSource& rom);  // error text or nullptr
  void stop();
  bool running() const { return running_; }

  void frame(const InputState& in);  // core 0
  void presentService();             // core 1
  uint32_t frames() const { return frames_; }
  uint32_t framesShown() const { return shown_; }
  const a2600::Machine* machine() const { return m_; }

 private:
  static constexpr int kBatchRows = 4;
  Renderer& r_;
  a2600::Machine* m_ = nullptr;
  bool running_ = false;
  uint32_t frames_ = 0;
  uint8_t* buf_[2] = {};      // frames, a2600::kW x a2600::kH palette indices
  uint8_t* shown_buf_ = nullptr;  // what the panel shows (same format)
  uint16_t* batch_[2] = {};
  uint8_t* srcCol_ = nullptr;  // panel column -> source column
  int draw_ = 0;
  volatile int ready_ = -1;
  volatile bool firstFrame_ = true;
  volatile uint32_t shown_ = 0;
};
