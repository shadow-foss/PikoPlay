#pragma once
#include <stdint.h>

// The ILI9225 panel. The only code that uses TFT_eSPI.
class Display {
 public:
  static constexpr int W = 220;  // landscape logical width
  static constexpr int H = 176;  // landscape logical height
  void begin();
  void fillRect(int x, int y, int w, int h, uint16_t rgb565);
  void pushRect(int x, int y, int w, int h, const uint16_t* rgb565);  // host-endian RGB565
  // DMA variant: returns once the transfer has started; px must stay untouched until the next
  // pushRectAsync() or finishAsync(). Always end a series with finishAsync().
  void pushRectAsync(int x, int y, int w, int h, const uint16_t* rgb565);
  void finishAsync();

 private:
  bool dmaOk_ = false;
  bool dmaOpen_ = false;
};

inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
  return (uint16_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}
