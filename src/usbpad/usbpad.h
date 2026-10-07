#pragma once
#include <stdint.h>

// GAMEPAD mode: the handheld as a USB HID gamepad. The only code that uses the HID stack.
// attach() registers the gamepad once at boot, next to the serial port; adding it later would
// re-enumerate USB. Outside GAMEPAD mode the pad is idle; begin()/end() start and stop sending.
// HID buttons: A=1 (south) B=2 (east) Y=4 (north) X=5 (west) SELECT=11 START=12.
// D-pad: hat switch, or the X/Y stick axes.
class UsbPad {
 public:
  struct Report {
    uint32_t buttons = 0;  // bit n = HID button n+1
    uint8_t hat = 0;       // 0 centred, 1 up, 2 up-right ... 8 up-left (clockwise)
    int8_t x = 0, y = 0;   // -127..127
    bool operator==(const Report& o) const { return buttons == o.buttons && hat == o.hat && x == o.x && y == o.y; }
    bool operator!=(const Report& o) const { return !(*this == o); }
  };

  static void attach();  // once, early in boot
  void begin();
  void end();
  bool active() const { return active_; }
  bool hostReady() const;  // enumerated by a host and not suspended
  // `held` is the logical InputState mask; BTN_MENU (the SELECT+START chord) counts as both.
  void update(uint16_t held, bool dpadAsStick, uint64_t nowUs);
  static Report map(uint16_t held, bool dpadAsStick);
  const Report& last() const { return last_; }
  uint32_t reportsSent() const { return sent_; }

 private:
  void send(const Report& r, uint64_t nowUs);
  bool active_ = false;
  bool have_ = false;
  Report last_{};
  uint64_t lastSendUs_ = 0, changedUs_ = 0;
  uint32_t sent_ = 0;
};
