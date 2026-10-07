#include "emulation/content.h"
#include <ctype.h>

static bool hasExt(const char* path, const char* ext) {
  size_t lp = strlen(path), le = strlen(ext);
  if (lp < le) return false;
  for (size_t i = 0; i < le; i++)
    if (tolower((unsigned char)path[lp - le + i]) != ext[i]) return false;
  return true;
}

static bool plausibleAtariSize(uint32_t s) {
  return s == 2048 || s == 4096 || s == 8192 || s == 12288 || s == 16384 || s == 32768 || s == 65536;
}

const char* contentSystemDir(ContentType t) {
  switch (t) {
    case ContentType::Atari2600: return "atari";
    case ContentType::NES: return "nes";
    case ContentType::GB: return "gb";
    case ContentType::GBC: return "gbc";
    default: return "unknown";
  }
}

ContentType detectContent(const char* path, RomSource& rom) {
  uint8_t h[4] = {0};
  if (rom.read(0, h, 4) == 4 && h[0] == 'N' && h[1] == 'E' && h[2] == 'S' && h[3] == 0x1A)
    return ContentType::NES;

  // Game Boy: by extension, or (any name) by a valid cartridge header checksum at 0x14D.
  const bool gbExt = hasExt(path, ".gb") || hasExt(path, ".gbc") || hasExt(path, ".sgb");
  if (rom.size() >= 0x150) {
    uint8_t hdr[0x1A] = {0};  // 0x134..0x14D
    bool sumOk = false;
    if (rom.read(0x134, hdr, sizeof hdr) == sizeof hdr) {
      uint8_t x = 0;
      for (int i = 0; i < 0x19; i++) x = (uint8_t)(x - hdr[i] - 1);
      sumOk = x == hdr[0x19];
    }
    if (gbExt || sumOk) {
      uint8_t cgb = 0;
      rom.read(0x143, &cgb, 1);
      if (cgb == 0xC0) return ContentType::GBC;                       // CGB only
      if (cgb == 0x80 && hasExt(path, ".gbc")) return ContentType::GBC;  // dual-mode: trust ext
      return ContentType::GB;
    }
  }

  // Atari 2600 images have no header: extension (+ size for the ambiguous .bin).
  if (hasExt(path, ".a26") || hasExt(path, ".a2600")) return ContentType::Atari2600;
  if (hasExt(path, ".bin") && plausibleAtariSize(rom.size())) return ContentType::Atari2600;
  return ContentType::Unknown;
}
