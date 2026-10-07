#pragma once
#include <stdint.h>
#include "display/renderer.h"

// Screen shown in GAMEPAD mode: a controller whose buttons light up as they are sent to the PC,
// the host connection state, and the SELECT+START hold-to-exit bar. Redraws only on change.
class PadScreen {
 public:
  explicit PadScreen(Renderer& r) : r_(r) {}
  void invalidate() { valid_ = false; r_.invalidateDiff(); }
  // held: logical mask (BTN_MENU = SELECT+START held); exitPct: 0..100 of the exit hold.
  void draw(uint16_t held, bool hostReady, bool dpadAsStick, int exitPct);

 private:
  Renderer& r_;
  bool valid_ = false;
  uint16_t held_ = 0;
  bool host_ = false, stick_ = false;
  int exit_ = 0;
};
