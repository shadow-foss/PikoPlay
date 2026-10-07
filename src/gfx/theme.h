#pragma once
#include <stdint.h>
#include "display/display.h"

// One theme for the whole console (launcher, built-in games, the cat). Chosen in Settings.
// The launcher is brutalist: flat fills, hard 2 px rules, one loud accent per theme.
struct Theme {
  const char* name;
  uint16_t bg;       // screen background
  uint16_t text;     // primary text / bright objects (ball, paddle)
  uint16_t muted;    // secondary text
  uint16_t line;     // separators, inactive dots
  uint16_t surface;  // cards, HUD bar, panels
  uint16_t accent;   // the one loud colour: header band, card shadow, tags
  uint16_t ink;      // text drawn on top of the accent
  bool dark;
};
constexpr int kThemeCount = 4;
extern const Theme kThemes[kThemeCount];  // MONO, PAPER, TERMINAL, COBALT

// Console settings (all persisted in /saves/system.sav by the runtime).
extern uint8_t g_themeIndex;  // 0..kThemeCount-1
// Game Boy picture: 0 = FIT (scaled to 195x176, full height), 1 = SHARP (1:1, 160x144).
extern uint8_t g_gbScale;
// Colours for original (non-colour) Game Boy games. Colour games always use their own palettes.
constexpr int kGbPaletteCount = 4;
extern uint8_t g_gbPalette;  // 0 DMG GREEN, 1 POCKET GREY, 2 BACKLIT, 3 AMBER
extern const uint8_t kGbPalettes[kGbPaletteCount][4][3];
extern const char* const kGbPaletteNames[kGbPaletteCount];
// D-pad auto-repeat in menus: 0 SLOW, 1 NORMAL, 2 FAST.
constexpr int kMenuSpeedCount = 3;
extern uint8_t g_menuSpeed;
extern const char* const kMenuSpeedNames[kMenuSpeedCount];
// 1 = the home carousel skips emulator systems that have no ROMs.
extern uint8_t g_hideEmpty;
// GAMEPAD mode: 0 = D-pad as a hat switch, 1 = D-pad as the left stick's X/Y axes.
extern uint8_t g_padStick;

inline const Theme& theme() { return kThemes[g_themeIndex % kThemeCount]; }

// Accents shared by everything (readable on every theme).
namespace accent {
const uint16_t coral = rgb565(255, 105, 120);
const uint16_t amber = rgb565(255, 176, 50);
const uint16_t teal = rgb565(40, 190, 160);
const uint16_t green = rgb565(90, 200, 80);
const uint16_t blue = rgb565(80, 140, 250);
const uint16_t warn = rgb565(255, 72, 48);
}  // namespace accent
