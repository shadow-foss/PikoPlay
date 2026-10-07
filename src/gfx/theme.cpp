#include "gfx/theme.h"

const Theme kThemes[kThemeCount] = {
  // name       bg                 text                 muted                line                 surface             accent               ink                dark
  {"MONO",     rgb565(0, 0, 0),    rgb565(255, 255, 255), rgb565(128, 128, 128), rgb565(52, 52, 52),  rgb565(20, 20, 20),  rgb565(255, 230, 0),  rgb565(0, 0, 0),   true},
  {"PAPER",    rgb565(240, 236, 226), rgb565(0, 0, 0),  rgb565(110, 104, 94), rgb565(196, 190, 176), rgb565(226, 220, 206), rgb565(255, 64, 32), rgb565(0, 0, 0),   false},
  {"TERMINAL", rgb565(0, 8, 0),    rgb565(120, 255, 100), rgb565(48, 150, 44),  rgb565(16, 60, 16),  rgb565(6, 26, 6),    rgb565(120, 255, 100), rgb565(0, 8, 0),  true},
  {"COBALT",   rgb565(16, 40, 210), rgb565(255, 255, 255), rgb565(160, 176, 255), rgb565(60, 86, 236), rgb565(10, 28, 160), rgb565(255, 230, 0), rgb565(16, 40, 210), true},
};
uint8_t g_themeIndex = 0;
uint8_t g_gbScale = 0;
uint8_t g_gbPalette = 0;
uint8_t g_menuSpeed = 1;
uint8_t g_hideEmpty = 0;
uint8_t g_padStick = 0;

// Lightest to darkest.
const uint8_t kGbPalettes[kGbPaletteCount][4][3] = {
  {{224, 248, 208}, {136, 192, 112}, {52, 104, 86}, {8, 24, 32}},     // DMG green
  {{232, 232, 224}, {160, 160, 152}, {88, 88, 84}, {16, 16, 16}},     // Pocket grey
  {{150, 240, 255}, {60, 170, 220}, {20, 90, 150}, {4, 24, 56}},      // Light (backlit blue)
  {{255, 216, 120}, {232, 144, 32}, {136, 64, 8}, {40, 12, 0}},       // Amber
};
const char* const kGbPaletteNames[kGbPaletteCount] = {"DMG GREEN", "POCKET", "BACKLIT", "AMBER"};
const char* const kMenuSpeedNames[kMenuSpeedCount] = {"SLOW", "NORMAL", "FAST"};
