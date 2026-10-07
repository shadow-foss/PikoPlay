# Local changes to InfoNES (from pico-infones by shuichitakano, GPL-3.0; InfoNES by Jay's Factory)

Source: https://github.com/shuichitakano/pico-infones (directory `infones/`). License: `LICENSE` (GPL-3.0).
Changes for PikoPlay (RP2040 with 264 KB RAM, no audio):

1. pico-infones' `util::WorkMeter*` profiling calls are stubbed out.
2. `DoubleFrame` (2 x 256 x 240 WORD = 245 KB) is removed; frames are drawn line by line.
3. `RAM`, `SRAM`, `PPURAM`, `SPRRAM`, `ChrBuf` and `DRAM` are pointers instead of arrays. The
   port assigns them from memory shared with the other emulators (only one runs at a time).
   `DRAM` stays null: mapper 235 (its only user) is refused by the port.
4. `InfoNES_Cycle()` returns after every frame (`InfoNES_FrameDone`), so the host drives frames.
5. `wave_buffers` shrunk to 4 samples: the port reports a 0-sample sound buffer (no speaker),
   so no waveform is ever rendered; APU register state and length counters still update.
6. `AA_ABS` / `AA_ABS2` (K6502.cpp): the two operand-byte reads were unsequenced
   (`Read(PC++) | Read(PC++) << 8`); now explicitly low byte first. Same behaviour with today's
   GCC, but no longer compiler-dependent.
7. Mappers 5 (MMC5), 6, 19 (Namco 163), 85 (VRC7) and 188 are compiled out: their static buffers
   (64 KB, 32 KB, 8 KB, 256 KB, 8 KB) cannot fit in the RP2040's 264 KB. The port reports
   "UNSUPPORTED MAPPER" for them.
8. InfoNES_pAPU.cpp functions are not placed in RAM (no speaker: the renderer never runs).
9. InfoNES_pAPUInit cleared 735 bytes of each wave buffer, which are only 4 bytes here: it now
   clears `sizeof wave_buffers[0]`.
10. Upstream bug: `APU_Reg[0x4015]` (frame IRQ status) indexed a 0x18-byte array 16 KB out of
    bounds; it is now `APU_Reg[0x15]`. The array is 0x20 bytes because $4000-$401F writes store
    to `APU_Reg[wAddr & 0x1f]`.
11. The 132 `mapper/InfoNES_Mapper_NNN.cpp` files, which upstream `#include`s into
    `InfoNES_Mapper.cpp`, are pasted into that file in the same order (no code changes).
