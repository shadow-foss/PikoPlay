#include "emulation/atari2600/atari2600.h"
#include <string.h>

namespace a2600 {

// Classic Stella NTSC palette as RGB565, indexed by colour register >> 1.
#define RGB(c) (uint16_t)(((((c) >> 16) & 0xF8) << 8) | ((((c) >> 8) & 0xFC) << 3) | (((c) & 0xFF) >> 3))
const uint16_t kPalette565[128] = {
  RGB(0x000000), RGB(0x404040), RGB(0x6C6C6C), RGB(0x909090), RGB(0xB0B0B0), RGB(0xC8C8C8), RGB(0xDCDCDC), RGB(0xECECEC),
  RGB(0x444400), RGB(0x646410), RGB(0x848424), RGB(0xA0A034), RGB(0xB8B840), RGB(0xD0D050), RGB(0xE8E85C), RGB(0xFCFC68),
  RGB(0x702800), RGB(0x844414), RGB(0x985C28), RGB(0xAC783C), RGB(0xBC8C4C), RGB(0xCCA05C), RGB(0xDCB468), RGB(0xECC878),
  RGB(0x841800), RGB(0x983418), RGB(0xAC5030), RGB(0xC06848), RGB(0xD0805C), RGB(0xE09470), RGB(0xECA880), RGB(0xFCBC94),
  RGB(0x880000), RGB(0x9C2020), RGB(0xB03C3C), RGB(0xC05858), RGB(0xD07070), RGB(0xE08888), RGB(0xECA0A0), RGB(0xFCB4B4),
  RGB(0x78005C), RGB(0x8C2074), RGB(0xA03C88), RGB(0xB0589C), RGB(0xC070B0), RGB(0xD084C0), RGB(0xDC9CD0), RGB(0xECB0E0),
  RGB(0x480078), RGB(0x602090), RGB(0x783CA4), RGB(0x8C58B8), RGB(0xA070CC), RGB(0xB484DC), RGB(0xC49CEC), RGB(0xD4B0FC),
  RGB(0x140084), RGB(0x302098), RGB(0x4C3CAC), RGB(0x6858C0), RGB(0x7C70D0), RGB(0x9488E0), RGB(0xA8A0EC), RGB(0xBCB4FC),
  RGB(0x000088), RGB(0x1C209C), RGB(0x3840B0), RGB(0x505CC0), RGB(0x6874D0), RGB(0x7C8CE0), RGB(0x90A4EC), RGB(0xA4B8FC),
  RGB(0x00187C), RGB(0x1C3890), RGB(0x3854A8), RGB(0x5070BC), RGB(0x6888CC), RGB(0x7C9CDC), RGB(0x90B4EC), RGB(0xA4C8FC),
  RGB(0x002C5C), RGB(0x1C4C78), RGB(0x386890), RGB(0x5084AC), RGB(0x689CC0), RGB(0x7CB4D4), RGB(0x90CCE8), RGB(0xA4E0FC),
  RGB(0x00402C), RGB(0x1C5C48), RGB(0x387C64), RGB(0x509C80), RGB(0x68B494), RGB(0x7CD0AC), RGB(0x90E4C0), RGB(0xA4FCD4),
  RGB(0x003C00), RGB(0x205C20), RGB(0x407C40), RGB(0x5C9C5C), RGB(0x74B474), RGB(0x8CD08C), RGB(0xA4E4A4), RGB(0xB8FCB8),
  RGB(0x143800), RGB(0x345C1C), RGB(0x507C38), RGB(0x6C9850), RGB(0x84B468), RGB(0x9CCC7C), RGB(0xB4E490), RGB(0xC8FCA4),
  RGB(0x2C3000), RGB(0x4C501C), RGB(0x687034), RGB(0x848C4C), RGB(0x9CA864), RGB(0xB4C078), RGB(0xCCD488), RGB(0xE0EC9C),
  RGB(0x442800), RGB(0x644818), RGB(0x846830), RGB(0xA08444), RGB(0xB89C58), RGB(0xD0B46C), RGB(0xE8CC7C), RGB(0xFCE08C),
};
#undef RGB

namespace {
// Collision latches as 16 bits: register r (CXM0P..CXPPMM) bit 7 -> bit 2r+1, bit 6 -> bit 2r.
struct Tables {
  uint16_t coll[64];
  uint8_t sel[2][64];  // colour source per object mix: 0 BK, 1 PF, 2 P0, 3 P1, 4 BL; [1] = CTRLPF.2
  Tables() {
    for (int m = 0; m < 64; ++m) {
      const bool p0 = m & Tia::kP0, p1 = m & Tia::kP1, m0 = m & Tia::kM0, m1 = m & Tia::kM1, bl = m & Tia::kBL, pf = m & Tia::kPF;
      uint16_t c = 0;
      auto set = [&](int reg, bool b7, bool b6) { if (b7) c |= 1u << (2 * reg + 1); if (b6) c |= 1u << (2 * reg); };
      set(0, m0 && p1, m0 && p0);
      set(1, m1 && p0, m1 && p1);
      set(2, p0 && pf, p0 && bl);
      set(3, p1 && pf, p1 && bl);
      set(4, m0 && pf, m0 && bl);
      set(5, m1 && pf, m1 && bl);
      set(6, bl && pf, false);
      set(7, p0 && p1, m0 && m1);
      coll[m] = c;
      uint8_t s = 0;  // normal priority: players above playfield/ball
      if (pf) s = 1;
      if (bl) s = 4;
      if (p1 || m1) s = 3;
      if (p0 || m0) s = 2;
      sel[0][m] = s;
      s = 0;          // CTRLPF.2: playfield/ball above players
      if (p1 || m1) s = 3;
      if (p0 || m0) s = 2;
      if (pf) s = 1;
      if (bl) s = 4;
      sel[1][m] = s;
    }
  }
};
const Tables kT;

inline int copies(uint8_t n, uint16_t off[3]) {
  switch (n & 7) {
    case 1: off[0] = 0; off[1] = 16; return 2;
    case 2: off[0] = 0; off[1] = 32; return 2;
    case 3: off[0] = 0; off[1] = 16; off[2] = 32; return 3;
    case 4: off[0] = 0; off[1] = 64; return 2;
    case 6: off[0] = 0; off[1] = 32; off[2] = 64; return 3;
    default: off[0] = 0; return 1;
  }
}
inline int signedHm(uint8_t hm) { const int n = hm >> 4; return n >= 8 ? n - 16 : n; }
inline uint8_t moved(uint8_t pos, uint8_t hm) { return (uint8_t)(((int)pos - signedHm(hm) + 160) % 160); }
}  // namespace


void Tia::write(uint8_t reg, uint8_t v) {
  switch (reg & 0x3F) {
    case 0x00: {
      const bool on = v & 2;
      if (on && !vsync) { line = 0; linesThisFrame = 0; }
      if (!on && vsync) finishFrame();
      vsync = on;
      break;
    }
    case 0x01: vblank = v & 2; break;
    case 0x02: wsync = true; break;
    case 0x03: clock = 0; break;
    case 0x04: nusiz[0] = v; dirty_ |= kP0 | kM0; break;
    case 0x05: nusiz[1] = v; dirty_ |= kP1 | kM1; break;
    case 0x06: colup0 = v; break;
    case 0x07: colup1 = v; break;
    case 0x08: colupf = v; break;
    case 0x09: colubk = v; break;
    case 0x0A: ctrlpf = v; dirty_ |= kPF | kBL; break;
    case 0x0B: refp[0] = v & 8; dirty_ |= kP0; break;
    case 0x0C: refp[1] = v & 8; dirty_ |= kP1; break;
    case 0x0D: pf[0] = v; dirty_ |= kPF; break;
    case 0x0E: pf[1] = v; dirty_ |= kPF; break;
    case 0x0F: pf[2] = v; dirty_ |= kPF; break;
    case 0x10: posP[0] = playerPos(); dirty_ |= kP0 | kM0; break;
    case 0x11: posP[1] = playerPos(); dirty_ |= kP1 | kM1; break;
    case 0x12: posM[0] = missilePos(); dirty_ |= kM0; break;
    case 0x13: posM[1] = missilePos(); dirty_ |= kM1; break;
    case 0x14: posBL = missilePos(); dirty_ |= kBL; break;
    case 0x1B: grp[0] = v; grpOld[1] = grp[1]; dirty_ |= kP0 | kP1; break;
    case 0x1C: grp[1] = v; grpOld[0] = grp[0]; enablOld = enabl; dirty_ |= kP0 | kP1 | kBL; break;
    case 0x1D: enam[0] = v & 2; dirty_ |= kM0; break;
    case 0x1E: enam[1] = v & 2; dirty_ |= kM1; break;
    case 0x1F: enabl = v & 2; dirty_ |= kBL; break;
    case 0x20: hmP[0] = v; break;
    case 0x21: hmP[1] = v; break;
    case 0x22: hmM[0] = v; break;
    case 0x23: hmM[1] = v; break;
    case 0x24: hmBL = v; break;
    case 0x25: vdelp[0] = v & 1; dirty_ |= kP0; break;
    case 0x26: vdelp[1] = v & 1; dirty_ |= kP1; break;
    case 0x27: vdelbl = v & 1; dirty_ |= kBL; break;
    case 0x28: resmp[0] = v & 2; dirty_ |= kM0; break;
    case 0x29: resmp[1] = v & 2; dirty_ |= kM1; break;
    case 0x2A:  // HMOVE
      for (int i = 0; i < 2; ++i) { posP[i] = moved(posP[i], hmP[i]); posM[i] = moved(posM[i], hmM[i]); }
      posBL = moved(posBL, hmBL);
      dirty_ |= kP0 | kP1 | kM0 | kM1 | kBL;
      if (clock < 68) hmoveBlank = true;  // the 8-pixel comb at the left edge
      break;
    case 0x2B: hmP[0] = hmP[1] = hmM[0] = hmM[1] = hmBL = 0; break;
    case 0x2C: coll = 0; break;
    default: break;
  }
}

uint8_t Tia::read(uint8_t reg) const {
  reg &= 0x0F;
  if (reg < 8) return (uint8_t)((((coll >> (2 * reg + 1)) & 1) << 7) | (((coll >> (2 * reg)) & 1) << 6));
  if (reg == 0x0C) return fire[0] ? 0x00 : 0x80;  // INPT4: fire pressed = bit 7 low
  if (reg == 0x0D) return fire[1] ? 0x00 : 0x80;
  return 0x80;
}

void Tia::rebuild() {
  const uint8_t d = dirty_;
  dirty_ = 0;
  for (int i = 0; i < 2; ++i)  // missiles locked to their player's centre (RESMP)
    if (resmp[i]) posM[i] = (uint8_t)((posP[i] + 4) % 160);
  const uint8_t keep = (uint8_t)~d;
  for (int x = 0; x < kW; ++x) mask_[x] &= keep;
  uint16_t off[3];
  for (int i = 0; i < 2; ++i) {
    const uint8_t pbit = i ? kP1 : kP0, mbit = i ? kM1 : kM0;
    if (d & pbit) {
      const uint8_t g = vdelp[i] ? grpOld[i] : grp[i];
      if (g) {
        const int n = copies(nusiz[i], off);
        const int scale = (nusiz[i] & 7) == 5 ? 2 : (nusiz[i] & 7) == 7 ? 4 : 1;
        for (int k = 0; k < n; ++k) {
          const int start = (posP[i] + off[k]) % 160;
          for (int dd = 0; dd < 8 * scale; ++dd) {
            const int bit = dd / scale;
            if ((g >> (refp[i] ? bit : 7 - bit)) & 1) mask_[(start + dd) % 160] |= pbit;
          }
        }
      }
    }
    if ((d & mbit) && enam[i] && !resmp[i]) {
      const int n = copies(nusiz[i], off);
      const int size = 1 << ((nusiz[i] >> 4) & 3);
      for (int k = 0; k < n; ++k) {
        const int start = (posM[i] + off[k]) % 160;
        for (int dd = 0; dd < size; ++dd) mask_[(start + dd) % 160] |= mbit;
      }
    }
  }
  if ((d & kBL) && (vdelbl ? enablOld : enabl)) {
    const int size = 1 << ((ctrlpf >> 4) & 3);
    for (int dd = 0; dd < size; ++dd) mask_[(posBL + dd) % 160] |= kBL;
  }
  if (d & kPF) {
    uint32_t bits = 0;  // 20 playfield bits, left to right
    for (int i = 0; i < 4; ++i) if ((pf[0] >> (4 + i)) & 1) bits |= 1u << i;
    for (int i = 0; i < 8; ++i) if ((pf[1] >> (7 - i)) & 1) bits |= 1u << (4 + i);
    for (int i = 0; i < 8; ++i) if ((pf[2] >> i) & 1) bits |= 1u << (12 + i);
    const bool reflect = ctrlpf & 1;
    for (int idx = 0; idx < 40; ++idx) {
      const int b = idx < 20 ? idx : reflect ? 39 - idx : idx - 20;
      if ((bits >> b) & 1) {
        uint8_t* m = mask_ + idx * 4;
        m[0] |= kPF; m[1] |= kPF; m[2] |= kPF; m[3] |= kPF;
      }
    }
  }
}

void Tia::drawSpan(int x0, int x1) {
  const int row = (int)line - kFirstLine;
  uint8_t* dst = (out && row >= 0 && row < kH) ? out + row * kW : scratchRow_;
  if (vblank) { memset(dst + x0, 0, (size_t)(x1 - x0)); return; }
  if (dirty_) rebuild();
  if (hmoveBlank && x0 < 8) {
    const int e = x1 < 8 ? x1 : 8;
    memset(dst + x0, 0, (size_t)(e - x0));
    x0 = e;
  }
  const uint8_t* sel = kT.sel[(ctrlpf >> 2) & 1];
  uint8_t cols[5] = {(uint8_t)(colubk >> 1), (uint8_t)(colupf >> 1), (uint8_t)(colup0 >> 1), (uint8_t)(colup1 >> 1),
                     (uint8_t)(colupf >> 1)};
  const bool score = ctrlpf & 2;
  uint16_t c = coll;
  for (int x = x0; x < x1; ++x) {
    const uint8_t m = mask_[x];
    c |= kT.coll[m];
    if (score) cols[1] = (uint8_t)((x < 80 ? colup0 : colup1) >> 1);
    dst[x] = cols[sel[m]];
  }
  coll = c;
}

void Tia::endLine() {
  clock = 0;
  ++line;
  ++linesThisFrame;
  wsync = false;
  hmoveBlank = false;
  if (linesThisFrame > 330) {  // no VSYNC for a long time: present what we have
    linesThisFrame = 0;
    line = 0;
    finishFrame();
  }
}

void Tia::finishFrame() { frameDone = true; }

void Tia::run(int clocks) {
  while (clocks > 0) {
    if (clock < 68) {
      const int n = clocks < 68 - clock ? clocks : 68 - clock;
      clock = (uint16_t)(clock + n);
      clocks -= n;
      continue;
    }
    const int n = clocks < 228 - clock ? clocks : 228 - clock;
    drawSpan(clock - 68, clock - 68 + n);
    clock = (uint16_t)(clock + n);
    clocks -= n;
    if (clock == 228) endLine();
  }
}


uint8_t Riot::read(uint16_t addr) {
  if (!(addr & 0x200)) return ram[addr & 0x7F];
  if (!(addr & 4)) {
    switch (addr & 3) {
      case 0: return swcha;
      case 1: return ddra;
      case 2: return swchb;
      default: return 0;
    }
  }
  if (!(addr & 1)) return timer;
  const uint8_t flag = underflow ? 0x80 : 0;
  underflow = false;
  return flag;
}

void Riot::write(uint16_t addr, uint8_t v) {
  if (!(addr & 0x200)) { ram[addr & 0x7F] = v; return; }
  if (!(addr & 4)) { if ((addr & 3) == 1) ddra = v; return; }
  if (addr & 0x10) {
    static const uint16_t kIv[4] = {1, 8, 64, 1024};
    timer = v;
    interval = count = kIv[addr & 3];
    underflow = false;
  }
}


uint8_t Machine::bankCount() const {
  switch (cart) {
    case Cart::F8: return 2;
    case Cart::FA: return 3;
    case Cart::F6: return 4;
    case Cart::F4: return 8;
    default: return 1;
  }
}

void Machine::hotspot(uint16_t a) {
  uint16_t base;
  switch (cart) {
    case Cart::F8: case Cart::FA: base = 0x1FF8; break;
    case Cart::F6: base = 0x1FF6; break;
    case Cart::F4: base = 0x1FF4; break;
    default: return;
  }
  if (a >= base && a < base + bankCount()) bank = (uint8_t)(a - base);
}

uint8_t Machine::read(uint16_t addrIn) {
  const uint16_t addr = addrIn & 0x1FFF;
  if (addr & 0x1000) {
    hotspot(addr);
    if (cart == Cart::FA && (addr & 0x0F00) == 0x0100) return ram256_[addr & 0xFF];  // $1100-$11FF read port
    const uint16_t off = addr & 0x0FFF;
    if (cart == Cart::Rom2K) return rom_[off & 0x7FF];
    if (cart == Cart::Rom4K) return rom_[off];
    return rom_[bank * 0x1000u + off];
  }
  sync();
  if (!(addr & 0x80)) return tia.read((uint8_t)addr);
  return riot.read(addr);
}

void Machine::write(uint16_t addrIn, uint8_t v) {
  const uint16_t addr = addrIn & 0x1FFF;
  if (addr & 0x1000) {
    hotspot(addr);
    if (cart == Cart::FA && (addr & 0x0F00) == 0x0000) ram256_[addr & 0xFF] = v;  // $1000-$10FF write port
    return;
  }
  sync();
  if (!(addr & 0x80)) { tia.write((uint8_t)addr, v); return; }
  riot.write(addr, v);
}

void Machine::skipWsync() {
  sync();
  if (!tia.wsync) return;
  const uint32_t n = (uint32_t)(228 - tia.clock + 2) / 3;  // stalled cycles until the line ends
  tia.run((int)(n * 3));
  riot.advance(n);
  cpu.cycles += n;
}

const char* Machine::load(const uint8_t* rom, uint32_t size) {
  switch (size) {
    case 2048: cart = Cart::Rom2K; break;
    case 4096: cart = Cart::Rom4K; break;
    case 8192: cart = Cart::F8; break;
    case 12288: cart = Cart::FA; break;
    case 16384: cart = Cart::F6; break;
    case 32768: cart = Cart::F4; break;
    default: return "UNSUPPORTED ROM SIZE";
  }
  rom_ = rom;
  bank = (uint8_t)(bankCount() - 1);  // carts power up in their last bank (reset vector lives there)
  cpu.reset();
  return nullptr;
}

void Machine::setInput(const Input& in) {
  uint8_t a = 0xFF;
  if (in.right) a &= (uint8_t)~0x80;
  if (in.left) a &= (uint8_t)~0x40;
  if (in.down) a &= (uint8_t)~0x20;
  if (in.up) a &= (uint8_t)~0x10;
  riot.swcha = a;
  uint8_t b = 0x3F | 0x08;
  if (in.reset) b &= (uint8_t)~0x01;
  if (in.select) b &= (uint8_t)~0x02;
  riot.swchb = b;
  tia.fire[0] = in.fire;
}

void Machine::runFrame() {
  tia.frameDone = false;
  const uint32_t start = cpu.cycles;
  while (!tia.frameDone && cpu.cycles - start < 2u * 262 * 76) {
    if (tia.wsync) skipWsync();
    else cpu.step();
  }
  sync();
}

}  // namespace a2600
