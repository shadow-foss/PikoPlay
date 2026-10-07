#pragma once
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "display/display.h"

// 220x176 framebuffer, 1:1 with the panel. Draw a whole screen into fb(), then presentDiff()
// sends only the pixels that changed since the last presented frame (a full push takes ~15 ms).
// present() pushes everything and makes the next presentDiff() a full push.
class Renderer {
 public:
  explicit Renderer(Display& d) : d_(d) {}
  uint16_t* fb() { return fb_; }
  void clear(uint16_t c) { for (size_t i = 0; i < sizeof(fb_) / 2; i++) fb_[i] = c; }

  void present() {
    push(0, 0, Display::W, Display::H);
    shadowValid_ = false;
  }

  // Send only what differs from the last presented frame, in <= 8-row bands, one bounding box per
  // band. Returns the number of pixels pushed.
  uint32_t presentDiff() {
    if (!shadowValid_) {
      push(0, 0, Display::W, Display::H);
      memcpy(shadow_, fb_, sizeof(fb_));
      shadowValid_ = true;
      return (uint32_t)Display::W * Display::H;
    }
    uint32_t sent = 0;
    for (int y0 = 0; y0 < Display::H; y0 += 8) {
      const int rows = (Display::H - y0 < 8) ? Display::H - y0 : 8;
      // Which columns changed anywhere in this 8-row band.
      bool col[Display::W];
      bool any = false;
      for (int x = 0; x < Display::W; ++x) col[x] = false;
      for (int r = 0; r < rows; ++r) {
        const uint16_t* a = fb_ + (y0 + r) * Display::W;
        const uint16_t* b = shadow_ + (y0 + r) * Display::W;
        if (!memcmp(a, b, Display::W * 2)) continue;  // most rows are unchanged: one fast compare
        const uint32_t* a32 = (const uint32_t*)a;
        const uint32_t* b32 = (const uint32_t*)b;
        for (int x2 = 0; x2 < Display::W / 2; ++x2) {
          const uint32_t d = a32[x2] ^ b32[x2];
          if (!d) continue;
          if (d & 0xFFFF) col[2 * x2] = true;
          if (d >> 16) col[2 * x2 + 1] = true;
          any = true;
        }
      }
      if (!any) continue;
      // Push runs of changed columns; small gaps are bridged (a push call costs ~ a few pixels).
      int x = 0;
      while (x < Display::W) {
        while (x < Display::W && !col[x]) ++x;
        if (x == Display::W) break;
        int end = x;
        for (int gap = 0; end < Display::W; ++end) {
          if (col[end]) gap = 0;
          else if (++gap > kGapBridge) { end -= gap - 1; break; }
        }
        while (end > x && !col[end - 1]) --end;
        const int w = end - x;
        push(x, y0, w, rows);
        for (int r = 0; r < rows; ++r)
          memcpy(shadow_ + (y0 + r) * Display::W + x, fb_ + (y0 + r) * Display::W + x, (size_t)w * 2);
        sent += (uint32_t)w * rows;
        x = end;
      }
    }
    return sent;
  }
  void invalidateDiff() { shadowValid_ = false; }
  // The shadow buffer as raw memory for the emulators (free while a game owns the screen).
  uint8_t* shadowBytes() { shadowValid_ = false; return (uint8_t*)shadow_; }
  // Framebuffer + shadow as one 151 KB block for the emulators (nullptr if not contiguous).
  static constexpr size_t kArenaBytes = sizeof(uint16_t) * Display::W * Display::H * 2;
  uint8_t* arena() {
    shadowValid_ = false;
    return (uint8_t*)shadow_ == (uint8_t*)fb_ + sizeof fb_ ? (uint8_t*)fb_ : nullptr;
  }
  void pushPixels(int x, int y, int w, int h, const uint16_t* px) { d_.pushRect(x, y, w, h, px); }
  void pushPixelsAsync(int x, int y, int w, int h, const uint16_t* px) { d_.pushRectAsync(x, y, w, h, px); }
  void finishPixels() { d_.finishAsync(); }


 private:
  static constexpr int kGapBridge = 6;
  void push(int x, int y, int w, int h) {
    if (x < 0 || y < 0 || w <= 0 || h <= 0 || x + w > Display::W || y + h > Display::H) return;
    for (int row = 0; row < h; row += 8) {
      const int rows = (h - row < 8) ? h - row : 8;
      for (int j = 0; j < rows; ++j)
        memcpy(strip_ + j * w, fb_ + (y + row + j) * Display::W + x, (size_t)w * 2);
      d_.pushRect(x, y + row, w, rows, strip_);
    }
  }

  Display& d_;
  alignas(8) uint16_t fb_[Display::W * Display::H];
  alignas(4) uint16_t shadow_[Display::W * Display::H];
  bool shadowValid_ = false;
  uint16_t strip_[Display::W * 8];
};
