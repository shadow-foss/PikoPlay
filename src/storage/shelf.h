#pragma once
#include <stddef.h>
#include <stdint.h>
#include "storage/rom_source.h"

// The game shelf: installed ROMs kept in raw, memory-mapped flash so emulators run them in place
// (XIP) with no copy step. Every file under /roms/ lives here; LittleFS keeps saves and settings.
//
// Flash map (8 MB, see platformio.ini):
//   0x000000 firmware (max 384 KB) | 0x060000 index A | 0x061000 index B | 0x070000 ROM data
//   (64 KB aligned, up to 0x7B0000 = 7424 KB) | 0x7BF000 LittleFS (256 KB: saves, settings)
//   | 0x7FF000 EEPROM. Enough for e.g. one 4 MB, one 2 MB and one 1 MB game.
// The index is a 4 KB table written alternately to A and B with a sequence number and CRC, so a
// power cut while it is rewritten leaves the previous table valid. ROM data is written first and
// only a finished, verified upload gets an index entry.
namespace shelf {

constexpr uint32_t kIndexA = 0x060000;
constexpr uint32_t kIndexB = 0x061000;
constexpr uint32_t kDataStart = 0x070000;
constexpr uint32_t kDataEnd = 0x7B0000;
constexpr uint32_t kBlock = 0x10000;  // allocation + erase unit
constexpr uint32_t kCapacity = kDataEnd - kDataStart;
constexpr int kMaxEntries = 50;

struct Entry {
  uint32_t offset;  // flash offset of the data (kBlock aligned)
  uint32_t size;    // bytes
  uint32_t sig;     // identity: crc of path, size and the first 16 KB
  char path[64];    // e.g. /roms/gbc/Star Pilot.gbc
};

bool isShelfPath(const char* path);  // "/roms/..." (not the folder itself)
bool usable();                        // false if the firmware image grew into the shelf
int count();
const Entry* at(int i);
const Entry* find(const char* path);
const uint8_t* data(const Entry* e);
bool remove(const char* path);
void usage(uint32_t& capacity, uint32_t& used);
uint32_t largestFree();

// Sequential writer (PikoLink uploads, installs from an SD/LittleFS file).
// begin() reserves space (replacing an existing entry with the same path only on commit when
// there is room for both, otherwise the old copy is dropped first), write() takes the bytes in
// order, commit() verifies and publishes. Nothing is visible until commit() returns true.
class Writer {
 public:
  const char* begin(const char* path, uint32_t size);  // nullptr = ok, else a short reason
  bool write(const uint8_t* d, size_t n);
  bool commit();
  void abort() { active_ = false; }
  bool active() const { return active_; }
  uint32_t written() const { return done_; }

 private:
  bool flushPage();
  bool active_ = false;
  char path_[64] = {};
  uint32_t offset_ = 0, size_ = 0, done_ = 0;
  uint8_t page_[256];
  uint32_t pageFill_ = 0;
};

// Copy a ROM file into the shelf (progress(done, total) between chunks). Returns the entry.
const Entry* install(const char* path, RomSource& src, void (*progress)(uint32_t, uint32_t, void*), void* ctx);
// Identity of a ROM as stored in Entry::sig.
uint32_t signature(const char* path, RomSource& src);

// Raw flash access.
const uint8_t* flashBase();
void flashErase(uint32_t offset, uint32_t len);
void flashProgram(uint32_t offset, const uint8_t* data, uint32_t len);  // len: multiple of 256

}  // namespace shelf
