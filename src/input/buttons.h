#pragma once
#include <stdint.h>

// Logical controls. Games and emulators never see GPIOs, only this mask.
enum Button : uint16_t {
  BTN_UP = 1 << 0, BTN_DOWN = 1 << 1, BTN_LEFT = 1 << 2, BTN_RIGHT = 1 << 3,
  BTN_A = 1 << 4, BTN_B = 1 << 5, BTN_X = 1 << 6, BTN_Y = 1 << 7,
  BTN_START = 1 << 8, BTN_SELECT = 1 << 9,
  BTN_MENU = 1 << 10,  // reserved for the runtime; never forwarded to games/emulators
};
