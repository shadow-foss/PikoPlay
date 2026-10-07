#pragma once
#include <stdint.h>

// 6502 / 6507 CPU. Every bus access is one cycle (Bus::cycle()), so the video chip stays in step
// and instruction timing matches the real chip. Official opcodes plus the stable unofficial ones;
// KIL halts. BCD enables decimal mode.
//
// Bus: uint8_t read(uint16_t); void write(uint16_t, uint8_t); void cycle(); bool stalled();
template <class Bus, bool BCD>
class Mos6502 {
 public:
  enum : uint8_t { N = 0x80, V = 0x40, U = 0x20, B = 0x10, D = 0x08, I = 0x04, Z = 0x02, C = 0x01 };
  uint8_t a = 0, x = 0, y = 0, sp = 0xFD, p = U | I;
  uint16_t pc = 0;
  bool halted = false;
  uint32_t cycles = 0;

  explicit Mos6502(Bus& bus) : bus_(bus) {}

  void reset() {
    sp = 0xFD; p = U | I; halted = false; a = x = y = 0;
    const uint8_t lo = rd(0xFFFC), hi = rd(0xFFFD);
    pc = (uint16_t)(hi << 8 | lo);
  }

  void step() {
    if (bus_.stalled() || halted) { bus_.cycle(); ++cycles; return; }
    const uint8_t op = fetch();
    const uint8_t cc = op & 3, bbb = (op >> 2) & 7, aaa = op >> 5;
    if ((op & 0x1F) == 0x10) {  // branches
      const uint8_t f = (op >> 6) == 0 ? N : (op >> 6) == 1 ? V : (op >> 6) == 2 ? C : Z;
      branch(((p & f) != 0) == ((op & 0x20) != 0));
      return;
    }
    switch (op) {
      case 0x00: fetch(); interrupt(0xFFFE); return;  // BRK
      case 0x20: {  // JSR
        const uint8_t lo = fetch();
        rd((uint16_t)(0x100 | sp));
        push((uint8_t)(pc >> 8)); push((uint8_t)pc);
        const uint8_t hi = fetch();
        pc = (uint16_t)(hi << 8 | lo);
        return;
      }
      case 0x40: {  // RTI
        idle(); rd((uint16_t)(0x100 | sp));
        p = (uint8_t)((pull() & ~B) | U);
        const uint8_t lo = pull(), hi = pull();
        pc = (uint16_t)(hi << 8 | lo);
        return;
      }
      case 0x60: {  // RTS
        idle(); rd((uint16_t)(0x100 | sp));
        const uint8_t lo = pull(), hi = pull();
        pc = (uint16_t)(hi << 8 | lo);
        rd(pc); ++pc;
        return;
      }
      case 0x08: idle(); push(p | B | U); return;                                             // PHP
      case 0x28: idle(); rd((uint16_t)(0x100 | sp)); p = (uint8_t)((pull() & ~B) | U); return;  // PLP
      case 0x48: idle(); push(a); return;                                                      // PHA
      case 0x68: idle(); rd((uint16_t)(0x100 | sp)); a = pull(); nz(a); return;                // PLA
      case 0x4C: pc = fetch16(); return;                                                       // JMP abs
      case 0x6C: {  // JMP (ind), with the page-wrap bug
        const uint16_t ptr = fetch16();
        const uint8_t lo = rd(ptr), hi = rd((uint16_t)((ptr & 0xFF00) | ((ptr + 1) & 0xFF)));
        pc = (uint16_t)(hi << 8 | lo);
        return;
      }
      case 0x88: idle(); nz(--y); return;
      case 0xC8: idle(); nz(++y); return;
      case 0xCA: idle(); nz(--x); return;
      case 0xE8: idle(); nz(++x); return;
      case 0xA8: idle(); nz(y = a); return;
      case 0xAA: idle(); nz(x = a); return;
      case 0x98: idle(); nz(a = y); return;
      case 0x8A: idle(); nz(a = x); return;
      case 0xBA: idle(); nz(x = sp); return;
      case 0x9A: idle(); sp = x; return;
      case 0x18: idle(); p &= ~C; return;
      case 0x38: idle(); p |= C; return;
      case 0x58: idle(); p &= ~I; return;
      case 0x78: idle(); p |= I; return;
      case 0xB8: idle(); p &= ~V; return;
      case 0xD8: idle(); p &= ~D; return;
      case 0xF8: idle(); p |= D; return;
      case 0xEA: case 0x1A: case 0x3A: case 0x5A: case 0x7A: case 0xDA: case 0xFA: idle(); return;
      case 0x80: case 0x82: case 0x89: case 0xC2: case 0xE2: fetch(); return;
      case 0x04: case 0x44: case 0x64: readOperand(kZp); return;
      case 0x14: case 0x34: case 0x54: case 0x74: case 0xD4: case 0xF4: readOperand(kZpx); return;
      case 0x0C: readOperand(kAbs); return;
      case 0x1C: case 0x3C: case 0x5C: case 0x7C: case 0xDC: case 0xFC: readOperand(kAbx); return;
      case 0xEB: sbc(fetch()); return;
      case 0x02: case 0x12: case 0x22: case 0x32: case 0x42: case 0x52:
      case 0x62: case 0x72: case 0x92: case 0xB2: case 0xD2: case 0xF2: halted = true; return;
      default: break;
    }
    switch (cc) {
      case 1: {  // ORA AND EOR ADC STA LDA CMP SBC
        static const uint8_t kModes[8] = {kIzx, kZp, kImm, kAbs, kIzy, kZpx, kAby, kAbx};
        const uint8_t mode = kModes[bbb];
        if (aaa == 4) {
          if (mode == kImm) { readOperand(kImm); return; }
          wr(effAddr(mode, kWrite), a);
          return;
        }
        const uint8_t v = readOperand(mode);
        if (aaa == 5) { nz(a = v); return; }
        alu(aaa, v);
        return;
      }
      case 2: {  // ASL ROL LSR ROR STX LDX DEC INC
        if (bbb == 2 && aaa < 4) { idle(); a = rmwOp(aaa, a); return; }
        if (aaa == 4 || aaa == 5) {
          uint8_t mode;
          switch (bbb) {
            case 0: mode = kImm; break;
            case 1: mode = kZp; break;
            case 3: mode = kAbs; break;
            case 5: mode = kZpy; break;
            case 7: mode = kAby; break;
            default: return;
          }
          if (aaa == 4) {
            if (mode == kImm || mode == kAby) { readOperand(mode); return; }
            wr(effAddr(mode, kWrite), x);
            return;
          }
          nz(x = readOperand(mode));
          return;
        }
        uint8_t mode;
        switch (bbb) {
          case 1: mode = kZp; break;
          case 3: mode = kAbs; break;
          case 5: mode = kZpx; break;
          case 7: mode = kAbx; break;
          default: readOperand(kImm); return;
        }
        const uint16_t addr = effAddr(mode, kRmw);
        const uint8_t v = rd(addr);
        wr(addr, v);  // dummy write of the unmodified value
        wr(addr, rmwOp(aaa, v));
        return;
      }
      case 0: {  // BIT STY LDY CPY CPX
        uint8_t mode;
        switch (bbb) {
          case 0: mode = kImm; break;
          case 1: mode = kZp; break;
          case 3: mode = kAbs; break;
          case 5: mode = kZpx; break;
          case 7: mode = kAbx; break;
          default: return;
        }
        switch (aaa) {
          case 1: {  // BIT
            if (mode != kZp && mode != kAbs) { readOperand(mode); return; }
            const uint8_t v = readOperand(mode);
            p = (uint8_t)((p & ~(N | V | Z)) | (v & (N | V)) | ((v & a) ? 0 : Z));
            return;
          }
          case 4:  // STY
            if (mode == kImm || mode == kAbx) { readOperand(mode); return; }
            wr(effAddr(mode, kWrite), y);
            return;
          case 5: nz(y = readOperand(mode)); return;  // LDY
          case 6:
            if (mode == kZpx || mode == kAbx) { readOperand(mode); return; }
            cmp(y, readOperand(mode));
            return;
          case 7:
            if (mode == kZpx || mode == kAbx) { readOperand(mode); return; }
            cmp(x, readOperand(mode));
            return;
          default: readOperand(mode); return;
        }
      }
      default: unofficial(aaa, bbb); return;
    }
  }

 private:
  enum : uint8_t { kImm, kZp, kZpx, kZpy, kAbs, kAbx, kAby, kIzx, kIzy };
  enum : uint8_t { kRead, kWrite, kRmw };
  Bus& bus_;

  inline uint8_t rd(uint16_t addr) { const uint8_t v = bus_.read(addr); bus_.cycle(); ++cycles; return v; }
  inline void wr(uint16_t addr, uint8_t v) { bus_.write(addr, v); bus_.cycle(); ++cycles; }
  inline uint8_t fetch() { return rd(pc++); }
  inline uint16_t fetch16() { const uint8_t lo = fetch(), hi = fetch(); return (uint16_t)(hi << 8 | lo); }
  inline void push(uint8_t v) { wr((uint16_t)(0x100 | sp), v); --sp; }
  inline uint8_t pull() { ++sp; return rd((uint16_t)(0x100 | sp)); }
  inline void idle() { rd(pc); }
  inline void nz(uint8_t v) { p = (uint8_t)((p & ~(N | Z)) | (v & N) | (v ? 0 : Z)); }

  uint16_t effAddr(uint8_t mode, uint8_t kind) {
    switch (mode) {
      case kZp: return fetch();
      case kZpx: case kZpy: {
        const uint8_t base = fetch();
        rd(base);
        return (uint8_t)(base + (mode == kZpx ? x : y));
      }
      case kAbs: return fetch16();
      case kAbx: case kAby: {
        const uint16_t base = fetch16();
        const uint16_t eff = (uint16_t)(base + (mode == kAbx ? x : y));
        if (kind != kRead || (base & 0xFF00) != (eff & 0xFF00)) rd((uint16_t)((base & 0xFF00) | (eff & 0xFF)));
        return eff;
      }
      case kIzx: {
        uint8_t z = fetch();
        rd(z);
        z = (uint8_t)(z + x);
        const uint8_t lo = rd(z), hi = rd((uint8_t)(z + 1));
        return (uint16_t)(hi << 8 | lo);
      }
      default: {  // kIzy
        const uint8_t z = fetch();
        const uint8_t lo = rd(z), hi = rd((uint8_t)(z + 1));
        const uint16_t base = (uint16_t)(hi << 8 | lo);
        const uint16_t eff = (uint16_t)(base + y);
        if (kind != kRead || (base & 0xFF00) != (eff & 0xFF00)) rd((uint16_t)((base & 0xFF00) | (eff & 0xFF)));
        return eff;
      }
    }
  }
  uint8_t readOperand(uint8_t mode) { return mode == kImm ? fetch() : rd(effAddr(mode, kRead)); }

  void adc(uint8_t v) {
    const uint8_t c = p & C;
    if (BCD && (p & D)) {
      int al = (a & 0x0F) + (v & 0x0F) + c;
      if (al >= 0x0A) al = ((al + 0x06) & 0x0F) + 0x10;
      int s = (a & 0xF0) + (v & 0xF0) + al;
      const int inter = (int)(int8_t)(a & 0xF0) + (int)(int8_t)(v & 0xF0) + al;  // NMOS N/V source
      const uint16_t bin = (uint16_t)(a + v + c);
      p = (uint8_t)((p & ~(N | V | Z | C)) | ((bin & 0xFF) ? 0 : Z) | ((inter & 0x80) ? N : 0) |
                    ((inter < -128 || inter > 127) ? V : 0));
      if (s >= 0xA0) s += 0x60;
      if (s >= 0x100) p |= C;
      a = (uint8_t)(s & 0xFF);
      return;
    }
    const uint16_t r = (uint16_t)(a + v + c);
    const uint8_t res = (uint8_t)r;
    p = (uint8_t)((p & ~(N | V | Z | C)) | (r > 0xFF ? C : 0) | (((a ^ res) & (v ^ res) & 0x80) ? V : 0));
    a = res;
    nz(res);
  }
  void sbc(uint8_t v) {
    const uint8_t c = p & C;
    const uint8_t res = (uint8_t)(a - v - (1 - c));
    const bool carry = (uint16_t)a >= (uint16_t)(v + (1 - c));
    const bool ov = ((a ^ v) & (a ^ res) & 0x80) != 0;
    const uint8_t a0 = a;
    if (BCD && (p & D)) {
      int al = (a0 & 0x0F) - (v & 0x0F) + c - 1;
      if (al < 0) al = ((al - 0x06) & 0x0F) - 0x10;
      int s = (a0 & 0xF0) - (v & 0xF0) + al;
      if (s < 0) s -= 0x60;
      a = (uint8_t)s;
    } else {
      a = res;
    }
    p = (uint8_t)((p & ~(N | V | Z | C)) | (carry ? C : 0) | (ov ? V : 0) | (res & N) | (res ? 0 : Z));
  }
  void cmp(uint8_t reg, uint8_t v) {
    const uint8_t r = (uint8_t)(reg - v);
    p = (uint8_t)((p & ~(N | Z | C)) | (r & N) | (r ? 0 : Z) | (reg >= v ? C : 0));
  }
  void alu(uint8_t op, uint8_t v) {
    switch (op) {
      case 0: nz(a |= v); break;
      case 1: nz(a &= v); break;
      case 2: nz(a ^= v); break;
      case 3: adc(v); break;
      case 6: cmp(a, v); break;
      case 7: sbc(v); break;
      default: break;
    }
  }
  uint8_t rmwOp(uint8_t op, uint8_t v) {
    uint8_t res;
    switch (op) {
      case 0: p = (uint8_t)((p & ~C) | (v >> 7)); res = (uint8_t)(v << 1); break;
      case 1: { const uint8_t c = p & C; p = (uint8_t)((p & ~C) | (v >> 7)); res = (uint8_t)((v << 1) | c); break; }
      case 2: p = (uint8_t)((p & ~C) | (v & 1)); res = v >> 1; break;
      case 3: { const uint8_t c = (p & C) ? 0x80 : 0; p = (uint8_t)((p & ~C) | (v & 1)); res = (uint8_t)((v >> 1) | c); break; }
      case 6: res = (uint8_t)(v - 1); break;
      default: res = (uint8_t)(v + 1); break;
    }
    nz(res);
    return res;
  }
  void branch(bool take) {
    const int8_t off = (int8_t)fetch();
    if (!take) return;
    idle();
    const uint16_t old = pc, nw = (uint16_t)(old + off);
    if ((old & 0xFF00) != (nw & 0xFF00)) rd((uint16_t)((old & 0xFF00) | (nw & 0xFF)));
    pc = nw;
  }
  void interrupt(uint16_t vector) {  // BRK only: the 6507 has no interrupt lines
    push((uint8_t)(pc >> 8)); push((uint8_t)pc);
    push((uint8_t)((p | U | B)));
    p |= I;
    const uint8_t lo = rd(vector), hi = rd((uint16_t)(vector + 1));
    pc = (uint16_t)(hi << 8 | lo);
  }
  void unofficial(uint8_t aaa, uint8_t bbb) {
    uint8_t mode;
    switch (bbb) {
      case 0: mode = kIzx; break;
      case 1: mode = kZp; break;
      case 3: mode = kAbs; break;
      case 4: mode = kIzy; break;
      case 5: mode = (aaa == 4 || aaa == 5) ? kZpy : kZpx; break;
      case 6: mode = kAby; break;
      case 7: mode = aaa == 5 ? kAby : kAbx; break;
      default: return;  // immediate-mode unofficials are not supported
    }
    if (aaa == 4) { wr(effAddr(mode, kWrite), a & x); return; }  // SAX
    if (aaa == 5) { const uint8_t v = readOperand(mode); a = x = v; nz(v); return; }  // LAX
    const uint16_t addr = effAddr(mode, kRmw);
    const uint8_t v = rd(addr);
    wr(addr, v);
    uint8_t res;
    switch (aaa) {
      case 0: res = rmwOp(0, v); alu(0, res); break;  // SLO
      case 1: res = rmwOp(1, v); alu(1, res); break;  // RLA
      case 2: res = rmwOp(2, v); alu(2, res); break;  // SRE
      case 3: res = rmwOp(3, v); adc(res); break;     // RRA
      case 6: res = (uint8_t)(v - 1); cmp(a, res); break;  // DCP
      default: res = (uint8_t)(v + 1); sbc(res); break;    // ISB
    }
    wr(addr, res);
  }
};
