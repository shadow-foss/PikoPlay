#include "emulation/atari_player.h"
#include <new>
#include <string.h>

using a2600::kPalette565;

const char* AtariPlayer::start(RomSource& rom) {
  running_ = false;
  const uint32_t size = rom.size();
  if (size > 32768) return "UNSUPPORTED ROM SIZE";
  constexpr uint32_t kFrame = a2600::kW * a2600::kH;  // 30,720 B
  constexpr uint32_t kBatch = kBatchRows * kW * 2;
  constexpr uint32_t kMachine = (sizeof(a2600::Machine) + 7) & ~7u;
  static_assert(kMachine + 32768 + 3 * kFrame + 2 * kBatch + kW <= Renderer::kArenaBytes, "Atari arena");
  r_.clear(0);
  r_.present();
  uint8_t* p = r_.arena();
  if (!p) return "NO MEMORY";
  m_ = new (p) a2600::Machine();  p += kMachine;
  uint8_t* romCopy = p;           p += 32768;
  buf_[0] = p;                    p += kFrame;
  buf_[1] = p;                    p += kFrame;
  shown_buf_ = p;                 p += kFrame;
  batch_[0] = (uint16_t*)p;       p += kBatch;
  batch_[1] = (uint16_t*)p;       p += kBatch;
  srcCol_ = p;
  if (rom.read(0, romCopy, size) != size) { m_ = nullptr; return "ROM READ FAILED"; }
  for (int dx = 0; dx < kW; ++dx) srcCol_[dx] = (uint8_t)(dx * a2600::kW / kW);
  memset(buf_[0], 0, 2 * kFrame);
  memset(shown_buf_, 0xFF, kFrame);  // matches nothing: the first frame is sent whole
  m_->tia.out = buf_[0];
  if (const char* e = m_->load(romCopy, size)) { m_ = nullptr; return e; }
  frames_ = shown_ = 0;
  draw_ = 0;
  ready_ = -1;
  firstFrame_ = true;
  running_ = true;
  return nullptr;
}

void AtariPlayer::stop() {
  running_ = false;
  while (ready_ >= 0) {}
  r_.finishPixels();
  if (m_) m_->~Machine();
  m_ = nullptr;
  r_.invalidateDiff();
}

void AtariPlayer::frame(const InputState& in) {
  if (!running_) return;
  a2600::Input ai;
  ai.up = in.held & BTN_UP;
  ai.down = in.held & BTN_DOWN;
  ai.left = in.held & BTN_LEFT;
  ai.right = in.held & BTN_RIGHT;
  ai.fire = in.held & (BTN_A | BTN_B);
  ai.reset = in.held & BTN_START;
  ai.select = in.held & BTN_SELECT;
  m_->setInput(ai);
  m_->tia.out = buf_[draw_];
  m_->runFrame();
  ++frames_;
  if (ready_ < 0) {
    __sync_synchronize();
    ready_ = draw_;
    draw_ ^= 1;
  }
}

// Changed source rows are widened to 220 columns and sent in batches of consecutive rows over the
// union of their changed columns (DMA; the next batch is converted while one is on the wire).
void AtariPlayer::presentService() {
  const int f = ready_;
  if (!running_ || f < 0) return;
  const uint8_t* src = buf_[f] + kCropTop * a2600::kW;
  uint8_t* shown = shown_buf_ + kCropTop * a2600::kW;
  int y = 0, b = 0;
  while (y < kH) {
    int s0 = a2600::kW, s1 = -1;  // changed source columns over the batch
    const int y0 = y;
    while (y < kH && y - y0 < kBatchRows) {
      const uint8_t* a = src + y * a2600::kW;
      const uint8_t* o = shown + y * a2600::kW;
      if (!memcmp(a, o, a2600::kW)) break;
      int x0 = 0, x1 = a2600::kW - 1;
      while (a[x0] == o[x0]) ++x0;
      while (a[x1] == o[x1]) --x1;
      if (x0 < s0) s0 = x0;
      if (x1 > s1) s1 = x1;
      ++y;
    }
    if (y == y0) { ++y; continue; }  // unchanged row
    int d0 = s0 * kW / a2600::kW, d1 = ((s1 + 1) * kW + a2600::kW - 1) / a2600::kW - 1;
    if (d1 >= kW) d1 = kW - 1;
    const int w = d1 - d0 + 1;
    uint16_t* out = batch_[b];
    for (int r = y0; r < y; ++r) {
      const uint8_t* a = src + r * a2600::kW;
      for (int x = d0; x <= d1; ++x) *out++ = kPalette565[a[srcCol_[x]] & 127];
      memcpy(shown + r * a2600::kW + s0, a + s0, (size_t)(s1 - s0 + 1));
    }
    r_.pushPixelsAsync(d0, y0, w, y - y0, batch_[b]);
    b ^= 1;
  }
  r_.finishPixels();
  firstFrame_ = false;
  ++shown_;
  __sync_synchronize();
  ready_ = -1;
}
