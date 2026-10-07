#pragma once
#include <stdint.h>

// 32 KB shared by the emulators (only one runs at a time):
//   Game Boy Color: cartridge RAM.   NES: CPU RAM 8 KB + battery RAM 8 KB + PPU RAM 16 KB.
// Outside games the ROM shelf borrows 4 KB of it to stage index rewrites (storage/shelf.cpp).
alignas(4) extern uint8_t g_emuPool[32768];
