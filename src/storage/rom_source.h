#pragma once
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// What an emulator sees of a ROM. It never learns which physical backend holds it, and it
// must never assume the image is fully in RAM.
class RomSource {
 public:
  virtual ~RomSource() {}
  virtual uint32_t size() const = 0;
  // Random-access chunk read; returns bytes read (short at EOF).
  virtual size_t read(uint32_t offset, void* dst, size_t len) = 0;
  // Non-null only if the whole image is directly addressable (XIP flash, RAM).
  // Cores must work without it (fall back to read()).
  virtual const uint8_t* map() const { return nullptr; }
};
