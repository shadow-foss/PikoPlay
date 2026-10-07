#pragma once
#include <stddef.h>
#include <stdint.h>
#include "gfx/font5x7.h"

// Clipped RGB565 drawing into a caller-owned pixel buffer (stride == width). No display access:
// callers present the rectangles they touched. Text is transparent (set pixels only), so fill
// a background first.
struct Canvas {
  uint16_t* px;
  int w, h;

  void fill(int x, int y, int rw, int rh, uint16_t c) {
    if (x < 0) { rw += x; x = 0; }
    if (y < 0) { rh += y; y = 0; }
    if (x + rw > w) rw = w - x;
    if (y + rh > h) rh = h - y;
    if (rw <= 0 || rh <= 0) return;
    for (int yy = y; yy < y + rh; ++yy) fill16(px + yy * w + x, rw, c);
  }
  // Fill n pixels with 32-bit stores where possible (about 2x faster than 16-bit on the M0+).
  static void fill16(uint16_t* p, int n, uint16_t c) {
    if (((uintptr_t)p & 2) && n > 0) { *p++ = c; --n; }
    uint32_t* q = (uint32_t*)p;
    const uint32_t cc = (uint32_t)c | ((uint32_t)c << 16);
    int pairs = n >> 1;
    while (pairs >= 4) { q[0] = cc; q[1] = cc; q[2] = cc; q[3] = cc; q += 4; pairs -= 4; }
    while (pairs-- > 0) *q++ = cc;
    if (n & 1) *(uint16_t*)q = c;
  }
  // Rectangle with the corner pixels cut (r = 1..3): a soft, cheap rounded look.
  void fillRound(int x, int y, int rw, int rh, int r, uint16_t c) {
    for (int i = 0; i < r; ++i) {
      const int inset = r - i;  // rows near the top/bottom are narrower
      fill(x + inset, y + i, rw - 2 * inset, 1, c);
      fill(x + inset, y + rh - 1 - i, rw - 2 * inset, 1, c);
    }
    fill(x, y + r, rw, rh - 2 * r, c);
  }
  void dot(int x, int y, uint16_t c) { if (x >= 0 && y >= 0 && x < w && y < h) px[y * w + x] = c; }
  void frame(int x, int y, int rw, int rh, uint16_t c) {
    fill(x, y, rw, 1, c); fill(x, y + rh - 1, rw, 1, c);
    fill(x, y, 1, rh, c); fill(x + rw - 1, y, 1, rh, c);
  }

  static int textWidth(const char* s, int scale = 1) {
    int n = 0;
    while (s[n]) ++n;
    return n ? n * 6 * scale - scale : 0;
  }
  void text(int x, int y, const char* s, uint16_t c, int scale = 1) {
    for (; *s; ++s, x += 6 * scale) {
      const uint8_t* g = kFont5x7[fontGlyph(*s)];
      for (int cx = 0; cx < 5; ++cx)
        for (int cy = 0; cy < 7; ++cy)
          if (g[cx] & (1u << cy)) fill(x + cx * scale, y + cy * scale, scale, scale, c);
    }
  }
  // Draw at most maxChars characters; returns false when it had to truncate.
  bool textClip(int x, int y, const char* s, uint16_t c, int maxChars, int scale = 1) {
    char buf[40];
    int n = 0;
    while (s[n] && n < maxChars && n < (int)sizeof(buf) - 1) { buf[n] = s[n]; ++n; }
    buf[n] = 0;
    text(x, y, buf, c, scale);
    return s[n] == 0;
  }
  void textRight(int rx, int y, const char* s, uint16_t c, int scale = 1) { text(rx - textWidth(s, scale), y, s, c, scale); }
  void textCenter(int cx, int y, const char* s, uint16_t c, int scale = 1) { text(cx - textWidth(s, scale) / 2, y, s, c, scale); }

  // 1-bit sprite; row r is bit (cols-1-x) of rows[r]. Set bits draw, clear bits are transparent.
  void sprite(int x, int y, const uint16_t* rows, int cols, int nrows, uint16_t c, int scale = 1) {
    for (int r = 0; r < nrows; ++r)
      for (int col = 0; col < cols; ++col)
        if (rows[r] & (1u << (cols - 1 - col))) fill(x + col * scale, y + r * scale, scale, scale, c);
  }
};

// 50/50 blend toward another colour, used for dimmed neighbour cards.
inline uint16_t blend565(uint16_t a, uint16_t b) {
  return (uint16_t)((((a & 0xF7DE) >> 1) + ((b & 0xF7DE) >> 1)) & 0xFFFF);
}
