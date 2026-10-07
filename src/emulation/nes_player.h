#pragma once
#include <stddef.h>
#include <stdint.h>
#include "display/renderer.h"
#include "input/input.h"
#include "storage/rom_source.h"

// NES player, built on InfoNES (lib/infones). Core 0 runs one frame per call; core 1
// (presentService) scales it from 256x224 to 220x176 with a bilinear filter and sends it to the
// screen. Its buffers live in the idle framebuffer and in g_emuPool.
class NesPlayer {
 public:
  static constexpr int kW = Display::W, kH = Display::H;
  static constexpr int kSrcW = 256, kSrcTop = 8, kSrcLines = 224;
  static constexpr size_t kSaveSize = 8192;

  explicit NesPlayer(Renderer& r) : r_(r) {}
  const char* start(const uint8_t* rom, uint32_t size);  // error text or nullptr
  void loadSave(RomSource& src);
  void stop();
  bool running() const { return running_; }
  bool hasBattery() const;

  void frame(const InputState& in);  // core 0
  void presentService();             // core 1
  uint32_t frames() const { return frames_; }
  uint32_t framesShown() const { return shown_; }

  const uint8_t* saveData() const;
  size_t saveSize() const { return hasBattery() ? kSaveSize : 0; }
  bool saveDirty() const;
  void clearSaveDirty();

  // Called by the InfoNES system hooks.
  uint16_t* lineBuffer() { return line_ + 32; }
  void postLine(int line);
  uint32_t pad() const { return pad_; }

 private:
  Renderer& r_;
  bool running_ = false;
  uint32_t frames_ = 0;
  uint32_t pad_ = 0;
  alignas(4) uint16_t line_[256 + 64];     // InfoNES draws 256 pixels at line_+32 (fine scroll spill room)
  static constexpr int kBatchRows = 4;
  uint8_t* buf_[2] = {};        // source frames, kSrcW x kSrcLines
  uint32_t* hash_[2] = {};      // per source line of each frame
  uint32_t* shownHash_ = nullptr;  // per source line of what the panel shows
  uint16_t* batch_[2] = {};     // scaled rows for DMA, kBatchRows x kW each (one fills, one sends)
  uint16_t* colTab_ = nullptr;  // per panel column: source column << 5 | weight of the next one
  uint16_t* rowTab_ = nullptr;  // per panel row: source line << 5 | weight of the next one
  int draw_ = 0;
  volatile int ready_ = -1;
  volatile bool firstFrame_ = true;
  volatile uint32_t shown_ = 0;
};
