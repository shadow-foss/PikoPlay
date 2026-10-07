#pragma once
#include <stdint.h>

namespace board {
constexpr int kButtonCount = 11;
constexpr bool kActiveLow = true;  // INPUT_PULLUP, button shorts to GND
constexpr uint8_t kNoPin = 0xFF;

// GPIO per logical button, in the bit order of input/input.h:
//   UP DOWN LEFT RIGHT A B X Y START SELECT MENU
//   D-pad UP GP7, DOWN GP8, LEFT GP12, RIGHT GP13; A (bottom) GP25, B (right) GP26,
//   X (left) GP24, Y (top) GP23; START GP21, SELECT GP20.
constexpr uint8_t kButtonPins[kButtonCount] = {7, 8, 12, 13, 25, 26, 24, 23, 21, 20, kNoPin};

// Pins owned by the shared display + SD SPI0 bus (see platformio.ini): buttons must never use them.
constexpr uint8_t kSpiPins[] = {0, 1, 2, 3, 4, 5, 6};  // MISO, SD_CS, SCK, MOSI, LCD_CS, LCD_DC, LCD_RST

// No dedicated MENU button on this board: MENU is synthesized from SELECT+START held together
// (and those two are withheld from games while the chord is down). Assign a pin above to
// get a real MENU button; the chord turns itself off.
constexpr bool kMenuIsChord = kButtonPins[10] == kNoPin;
}  // namespace board
