#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "storage/shelf.h"
#include "storage/storage.h"

// PikoLink: the USB serial file protocol used by tools/pikolink. Frames start with A5 5A, then
// cmd, seq, len (u16), payload and a CRC-32; anything else on the port is ignored.

class Link {
 public:
  static constexpr size_t kMaxPayload = 2048 + 8;  // up to 2 KB of data per frame (RAM is shared with the emulators)
  void begin();
  // Remove upload temp files (*.part) left behind by an interrupted transfer or a power cut.
  static int sweepTemp(Storage& storage);
  // Pump the port. `busy` = a game/emulator is running (mutating commands are refused).
  void poll(uint64_t nowUs, Storage& storage, bool busy);

  // Helpers.
  static bool pathOk(const char* p);
  // LAUNCH (0x0B): a path the runtime should start (empty when none). Cleared by takeLaunch().
  bool takeLaunch(char* out, size_t cap) {
    if (!launch_[0]) return false;
    snprintf(out, cap, "%s", launch_);
    launch_[0] = 0;
    return true;
  }

 private:
  void feed(uint8_t c, uint64_t nowUs, Storage& storage, bool busy);
  void handle(uint8_t cmd, uint8_t seq, const uint8_t* p, uint16_t n, Storage& storage, bool busy);
  void reply(uint8_t cmd, uint8_t seq, const uint8_t* p, size_t n);
  void error(uint8_t seq, uint8_t code, const char* msg);
  void abortUpload(Storage& storage);

  enum State : uint8_t { kIdle, kSync2, kHead, kBody, kCrc } state_ = kIdle;
  uint8_t head_[4] = {};
  uint8_t headN_ = 0;
  uint16_t len_ = 0, got_ = 0;
  uint8_t crc_[4] = {};
  uint8_t crcN_ = 0;
  uint64_t frameStartUs_ = 0;
  uint8_t buf_[kMaxPayload];
  // Upload in progress
  bool up_ = false;
  char upPath_[64] = {}, upTmp_[72] = {};
  uint32_t upSize_ = 0, upOff_ = 0, upCrc_ = 0;
  fs::FS* upFs_ = nullptr;
  fs::File upFile_;
  bool upShelf_ = false;  // /roms/* uploads stream straight onto the ROM shelf
  shelf::Writer shelfUp_;
  char launch_[64] = {};
};
