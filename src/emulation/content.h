#pragma once
#include "storage/rom_source.h"

enum class ContentType : uint8_t { Unknown, Atari2600, NES, GB, GBC };

const char* contentSystemDir(ContentType t);  // "gbc" (used for /saves/<system>/...)

// Header sniffing first (iNES magic; GB header checksum + CGB flag), extension as fallback.
ContentType detectContent(const char* path, RomSource& rom);

// A handheld core exists for this system (all four have one; the launcher shows NO CORE otherwise).
inline bool hasCore(ContentType t) {
  return t == ContentType::GB || t == ContentType::GBC || t == ContentType::NES || t == ContentType::Atari2600;
}
