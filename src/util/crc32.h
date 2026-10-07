#pragma once
#include <stddef.h>
#include <stdint.h>

// CRC-32 (IEEE 802.3, the zlib/PNG one). Used for save-change detection, the ROM shelf index and
// PikoLink transfers.
namespace piko {
inline uint32_t crc32Update(uint32_t c, const uint8_t* d, size_t n) {  // c starts at 0xFFFFFFFF
  for (size_t i = 0; i < n; ++i) {
    c ^= d[i];
    for (int k = 0; k < 8; ++k) c = (c >> 1) ^ (0xEDB88320u & (uint32_t)-(int32_t)(c & 1));
  }
  return c;
}
inline uint32_t crc32(const uint8_t* d, size_t n) { return ~crc32Update(0xFFFFFFFFu, d, n); }
}  // namespace piko
