#include "display/display.h"
#include <TFT_eSPI.h>
#include <hardware/clocks.h>
#include <hardware/spi.h>

static TFT_eSPI tft;

// The SD card shares SPI0 and its driver may leave the bus at ~400 kHz: restore the display
// clock before every draw.
static uint32_t g_displayBaud = 0;
static inline void claimBus() {
  if (spi_get_baudrate(spi0) != g_displayBaud) spi_set_baudrate(spi0, SPI_FREQUENCY);
}

void Display::begin() {
  // clk_peri defaults to the 48 MHz USB PLL, which caps SPI at 24 MHz. Clock it from the system
  // PLL instead. Must happen before tft.init() computes the SPI divider.
  const uint32_t sys = clock_get_hz(clk_sys);
  clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, sys, sys);
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);  // we hand over host-endian RGB565
  tft.fillScreen(0);
  dmaOk_ = tft.initDMA();
  g_displayBaud = spi_get_baudrate(spi0);
}
void Display::fillRect(int x, int y, int w, int h, uint16_t c) {
  claimBus();
  tft.fillRect(x, y, w, h, c);
}
void Display::pushRect(int x, int y, int w, int h, const uint16_t* px) {
  claimBus();
  tft.pushImage(x, y, w, h, px);
}
void Display::pushRectAsync(int x, int y, int w, int h, const uint16_t* px) {
  if (!dmaOk_) { pushRect(x, y, w, h, px); return; }
  if (!dmaOpen_) {
    claimBus();
    tft.startWrite();
    dmaOpen_ = true;
  }
  tft.dmaWait();
  tft.setAddrWindow(x, y, w, h);
  tft.pushPixelsDMA(const_cast<uint16_t*>(px), (uint32_t)(w * h));
}
void Display::finishAsync() {
  if (!dmaOpen_) return;
  tft.dmaWait();
  tft.endWrite();
  dmaOpen_ = false;
}
