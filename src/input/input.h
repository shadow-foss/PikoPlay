#pragma once
#include <stdint.h>
#include "input/buttons.h"

// One snapshot of the buttons for everyone. Masks contain BTN_* values.
struct InputState {
  uint16_t held = 0;
  uint16_t pressed = 0;    // went down since the previous poll
  uint16_t released = 0;   // went up since the previous poll
  uint16_t repeated = 0;   // pressed, plus D-pad auto-repeat while held (menus)
};

class Input {
 public:
  void begin();
  void poll(uint64_t nowUs);
  uint16_t held() const { return state_.held; }
  uint16_t pressed() const { return state_.pressed; }
  uint16_t released() const { return state_.released; }
  const InputState& state() const { return state_; }
  // D-pad auto-repeat timing (Settings > MENU SPEED).
  void setRepeat(uint32_t delayUs, uint32_t intervalUs) { repeatDelayUs_ = delayUs; repeatIntervalUs_ = intervalUs; }

 private:
  uint16_t sample() const;
  static constexpr uint32_t kLockoutUs = 30000;
  static constexpr int kButtonBits = 11;
  uint32_t repeatDelayUs_ = 350000;
  uint32_t repeatIntervalUs_ = 90000;
  uint16_t held_ = 0;
  uint64_t lockUntil_[kButtonBits] = {};
  uint64_t repeatAt_[4] = {};  // D-pad only
  InputState state_;
};
