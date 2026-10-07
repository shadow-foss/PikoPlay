#include "ui/pad_screen.h"
#include <stdio.h>
#include "gfx/canvas.h"
#include "gfx/theme.h"
#include "input/buttons.h"

namespace {
// A square key: ink-bordered, filled with the accent while pressed.
void key(Canvas& c, int x, int y, int w, int h, const char* label, bool on, const Theme& t) {
  c.fill(x, y, w, h, t.text);
  c.fill(x + 2, y + 2, w - 4, h - 4, on ? t.accent : t.bg);
  if (label) c.textCenter(x + w / 2, y + (h - 7) / 2, label, on ? t.ink : t.text);
}
}  // namespace

void PadScreen::draw(uint16_t held, bool hostReady, bool dpadAsStick, int exitPct) {
  if (held & BTN_MENU) held = (uint16_t)(held | BTN_SELECT | BTN_START);
  if (valid_ && held == held_ && hostReady == host_ && dpadAsStick == stick_ && exitPct == exit_) return;
  valid_ = true;
  held_ = held;
  host_ = hostReady;
  stick_ = dpadAsStick;
  exit_ = exitPct;

  Canvas c{r_.fb(), Display::W, Display::H};
  const Theme& t = theme();
  c.fill(0, 0, Display::W, Display::H, t.bg);
  c.fill(0, 0, Display::W, 18, t.accent);
  c.text(8, 6, "GAMEPAD", t.ink);
  c.textRight(Display::W - 8, 6, "USB HID", t.ink);

  // host state
  const char* st = hostReady ? "CONNECTED TO HOST" : "WAITING FOR A HOST";
  const int sw = Canvas::textWidth(st) + 12;
  c.fill(8, 24, sw, 13, hostReady ? accent::green : t.line);
  c.text(14, 27, st, hostReady ? rgb565(0, 0, 0) : t.text);

  // controller body with a hard shadow
  const int bx = 10, by = 44, bw = 196, bh = 88;
  c.fill(bx + 5, by + 5, bw, bh, t.accent);
  c.fill(bx, by, bw, bh, t.text);
  c.fill(bx + 2, by + 2, bw - 4, bh - 4, t.surface);
  // D-pad
  const int dx = 30, dy = 62, k = 16;
  key(c, dx + k, dy, k, k, nullptr, held & BTN_UP, t);
  key(c, dx, dy + k, k, k, nullptr, held & BTN_LEFT, t);
  key(c, dx + k, dy + k, k, k, nullptr, false, t);
  key(c, dx + 2 * k, dy + k, k, k, nullptr, held & BTN_RIGHT, t);
  key(c, dx + k, dy + 2 * k, k, k, nullptr, held & BTN_DOWN, t);
  // face buttons as on the handheld (Xbox layout): Y top, X left, B right, A bottom
  const int fx = 138, fy = 62;
  key(c, fx + k, fy, k, k, "Y", held & BTN_Y, t);
  key(c, fx, fy + k, k, k, "X", held & BTN_X, t);
  key(c, fx + 2 * k, fy + k, k, k, "B", held & BTN_B, t);
  key(c, fx + k, fy + 2 * k, k, k, "A", held & BTN_A, t);
  // select / start
  key(c, 84, 114, 26, 12, "SEL", held & BTN_SELECT, t);
  key(c, 114, 114, 36, 12, "START", held & BTN_START, t);

  c.text(8, 142, dpadAsStick ? "D-PAD = STICK" : "D-PAD = HAT", t.muted);
  c.textRight(Display::W - 8, 142, "A1 B2 Y4 X5 SL11 ST12", t.muted);

  // footer: hold-to-exit, the bar fills while SELECT+START are held
  c.fill(0, 158, Display::W, 2, t.text);
  if (exitPct > 0) {
    c.textCenter(Display::W / 2, 162, "KEEP HOLDING TO EXIT", t.accent);
    c.fill(0, Display::H - 5, Display::W * exitPct / 100, 5, t.accent);
  } else {
    c.textCenter(Display::W / 2, 162, "HOLD SELECT+START 1 S TO EXIT", t.text);
  }
  r_.presentDiff();
}
