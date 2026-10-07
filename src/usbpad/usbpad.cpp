#include "usbpad/usbpad.h"
#include "input/buttons.h"
#include <Joystick.h>
#include "tusb.h"

namespace {
// A report can be dropped when the HID endpoint is busy; repeat a change for a short while so a
// press or release is never lost, and send a slow heartbeat otherwise.
constexpr uint64_t kRepeatForUs = 60000, kRepeatEveryUs = 8000, kHeartbeatUs = 500000;
}  // namespace

UsbPad::Report UsbPad::map(uint16_t held, bool dpadAsStick) {
  if (held & BTN_MENU) held = (uint16_t)(held | BTN_SELECT | BTN_START);
  Report r;
  if (held & BTN_A) r.buttons |= 1u << 0;        // south
  if (held & BTN_B) r.buttons |= 1u << 1;        // east
  if (held & BTN_Y) r.buttons |= 1u << 3;        // north
  if (held & BTN_X) r.buttons |= 1u << 4;        // west
  if (held & BTN_SELECT) r.buttons |= 1u << 10;
  if (held & BTN_START) r.buttons |= 1u << 11;
  const int dx = ((held & BTN_RIGHT) ? 1 : 0) - ((held & BTN_LEFT) ? 1 : 0);
  const int dy = ((held & BTN_DOWN) ? 1 : 0) - ((held & BTN_UP) ? 1 : 0);
  if (dpadAsStick) {
    r.x = (int8_t)(dx * 127);
    r.y = (int8_t)(dy * 127);
  } else {
    // index (dy+1)*3 + (dx+1) -> hat value
    static const uint8_t kHat[9] = {8, 1, 2, 7, 0, 3, 6, 5, 4};
    r.hat = kHat[(dy + 1) * 3 + (dx + 1)];
  }
  return r;
}

void UsbPad::attach() {
  Joystick.use8bit(true);
  Joystick.useManualSend(true);
  Joystick.begin();
}

void UsbPad::begin() {
  if (active_) return;
  active_ = true;
  have_ = false;
}

void UsbPad::end() {
  if (!active_) return;
  send(Report{}, lastSendUs_);  // leave the host with nothing pressed
  active_ = false;
}

bool UsbPad::hostReady() const {
  return active_ && tud_mounted() && !tud_suspended();
}

void UsbPad::send(const Report& r, uint64_t nowUs) {
  for (int i = 0; i < 12; ++i) Joystick.setButton((uint8_t)i, (r.buttons >> i) & 1);
  Joystick.hat((HID_Joystick::HatPosition)r.hat);
  Joystick.position(r.x, r.y);
  Joystick.send_now();
  last_ = r;
  have_ = true;
  lastSendUs_ = nowUs;
  ++sent_;
}

void UsbPad::update(uint16_t held, bool dpadAsStick, uint64_t nowUs) {
  if (!active_) return;
  const Report r = map(held, dpadAsStick);
  if (!have_ || r != last_) {
    changedUs_ = nowUs;
    send(r, nowUs);
  } else if ((nowUs - changedUs_ < kRepeatForUs && nowUs - lastSendUs_ >= kRepeatEveryUs) || nowUs - lastSendUs_ >= kHeartbeatUs) {
    send(r, nowUs);
  }
}
