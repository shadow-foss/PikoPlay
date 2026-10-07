#include "input/input.h"
#include <Arduino.h>
#include "board.h"

void Input::begin() {
  for (int i = 0; i < board::kButtonCount; i++)
    if (board::kButtonPins[i] != board::kNoPin) pinMode(board::kButtonPins[i], INPUT_PULLUP);
}

uint16_t Input::sample() const {
  uint16_t m = 0;
  for (int i = 0; i < board::kButtonCount; i++) {
    if (board::kButtonPins[i] == board::kNoPin) continue;
    bool level = digitalRead(board::kButtonPins[i]) != 0;
    if (level != board::kActiveLow) m |= (uint16_t)(1u << i);
  }
  return m;  // physical buttons only; the MENU chord is applied after debouncing
}

// Debounce: a change is taken at once, then that button ignores changes for kLockoutUs.
void Input::poll(uint64_t now) {
  const uint16_t raw = sample();
  for (int b = 0; b < kButtonBits; ++b) {
    const uint16_t bit = (uint16_t)(1u << b);
    if ((raw & bit) == (held_ & bit)) continue;
    if (now < lockUntil_[b]) continue;
    held_ = (uint16_t)((held_ & ~bit) | (raw & bit));
    lockUntil_[b] = now + kLockoutUs;
  }

  // SELECT+START also raises MENU; the buttons themselves stay visible (games may use the combo).
  // The runtime leaves a game only when the chord is HELD for a second.
  uint16_t pad = held_;
  if (board::kMenuIsChord && (pad & BTN_SELECT) && (pad & BTN_START)) pad = (uint16_t)(pad | BTN_MENU);

  const uint16_t prev = state_.held;
  state_.held = pad;
  state_.pressed = state_.held & (uint16_t)~prev;
  state_.released = prev & (uint16_t)~state_.held;

  // Auto-repeat is D-pad only, so held A/START never re-triggers actions.
  state_.repeated = state_.pressed;
  for (int i = 0; i < 4; ++i) {
    const uint16_t bit = (uint16_t)(1u << i);
    if (state_.pressed & bit) repeatAt_[i] = now + repeatDelayUs_;
    else if ((state_.held & bit) && now >= repeatAt_[i]) {
      state_.repeated |= bit;
      repeatAt_[i] = now + repeatIntervalUs_;
    }
  }
}
