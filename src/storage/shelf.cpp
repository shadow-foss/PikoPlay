#include "storage/shelf.h"
#include <stdio.h>
#include <string.h>
#include "emulation/emu_pool.h"
#include "util/crc32.h"

#include <Arduino.h>
#include <hardware/flash.h>
#include <hardware/sync.h>
extern "C" char __flash_binary_end;
const uint8_t* shelf::flashBase() { return (const uint8_t*)XIP_BASE; }
// While the flash is busy nothing may run from it: interrupts are off and core 1 (the display
// presenter) is parked, as the core's LittleFS driver does. Work is split into 64 KB erases and
// 256 B programs so USB is never starved for long.
void shelf::flashErase(uint32_t off, uint32_t len) {
  for (uint32_t done = 0; done < len; done += 65536) {
    const uint32_t chunk = (len - done < 65536) ? len - done : 65536;
    noInterrupts();
    rp2040.idleOtherCore();
    flash_range_erase(off + done, chunk);
    rp2040.resumeOtherCore();
    interrupts();
  }
}
void shelf::flashProgram(uint32_t off, const uint8_t* d, uint32_t len) {
  for (uint32_t done = 0; done < len; done += 256) {
    noInterrupts();
    rp2040.idleOtherCore();
    flash_range_program(off + done, d + done, 256);
    rp2040.resumeOtherCore();
    interrupts();
  }
}
static bool firmwareFits() { return (uint32_t)(&__flash_binary_end - (const char*)XIP_BASE) <= shelf::kIndexA; }

namespace shelf {
namespace {
constexpr uint32_t kMagic = 0x31485350u;  // "PSH1"

struct Table {
  uint32_t magic;
  uint32_t seq;
  uint32_t count;
  uint32_t crc;  // over seq, count and the used entries
  Entry e[kMaxEntries];
};
static_assert(sizeof(Table) <= 4096, "index must fit one flash sector");

uint32_t tableCrc(const Table* t) {
  uint32_t c = piko::crc32((const uint8_t*)&t->seq, 8);
  return c * 31 + piko::crc32((const uint8_t*)t->e, t->count * sizeof(Entry));
}
const Table* tableAt(uint32_t off) {
  const Table* t = (const Table*)(flashBase() + off);
  if (t->magic != kMagic || t->count > (uint32_t)kMaxEntries || t->crc != tableCrc(t)) return nullptr;
  return t;
}
// The valid table with the higher sequence number (nullptr: empty shelf).
const Table* active() {
  const Table* a = tableAt(kIndexA);
  const Table* b = tableAt(kIndexB);
  if (a && b) return a->seq > b->seq ? a : b;
  return a ? a : b;
}
// Staging copy for index rewrites: 4 KB of the emulator pool (RAM is too tight for a static
// buffer). The shelf is only written while no game runs, so the pool is free.
static_assert(sizeof(Table) <= sizeof(g_emuPool), "staging table must fit the pool");
Table& g_tmp = *(Table*)(void*)g_emuPool;

bool publish() {
  const Table* cur = active();
  const uint32_t dst = cur == (const Table*)(flashBase() + kIndexA) ? kIndexB : kIndexA;
  g_tmp.magic = kMagic;
  g_tmp.crc = tableCrc(&g_tmp);
  flashErase(dst, 4096);
  const uint8_t* src = (const uint8_t*)&g_tmp;
  static uint8_t page[256];
  for (uint32_t o = 0; o < sizeof(Table); o += 256) {
    const uint32_t n = sizeof(Table) - o < 256 ? sizeof(Table) - o : 256;
    memset(page, 0xFF, 256);
    memcpy(page, src + o, n);
    flashProgram(dst + o, page, 256);
  }
  return tableAt(dst) != nullptr;
}
// Load the current table into g_tmp with the next sequence number.
void stage() {
  const Table* cur = active();
  if (cur) memcpy(&g_tmp, cur, sizeof g_tmp);
  else memset(&g_tmp, 0, sizeof g_tmp);
  g_tmp.seq = cur ? cur->seq + 1 : 1;
  if (!cur) g_tmp.count = 0;
}
uint32_t roundUp(uint32_t n) { return (n + kBlock - 1) & ~(kBlock - 1); }
// First gap of `need` bytes, ignoring the entry with path `skip` (nullptr: ignore none).
uint32_t allocate(uint32_t need, const char* skip) {
  const Table* t = active();
  uint32_t at = kDataStart;
  for (;;) {
    bool moved = false;
    for (uint32_t i = 0; t && i < t->count; ++i) {
      const Entry& e = t->e[i];
      if (skip && !strcmp(e.path, skip)) continue;
      const uint32_t end = e.offset + roundUp(e.size);
      if (at < end && e.offset < at + need) { at = end; moved = true; }
    }
    if (!moved) break;
  }
  return at + need <= kDataEnd ? at : 0;
}
}  // namespace

bool isShelfPath(const char* p) { return !strncmp(p, "/roms/", 6) && p[6]; }
bool usable() { return firmwareFits(); }
int count() { const Table* t = active(); return t ? (int)t->count : 0; }
const Entry* at(int i) { const Table* t = active(); return t && i >= 0 && i < (int)t->count ? &t->e[i] : nullptr; }
const Entry* find(const char* path) {
  const Table* t = active();
  for (uint32_t i = 0; t && i < t->count; ++i) if (!strcmp(t->e[i].path, path)) return &t->e[i];
  return nullptr;
}
const uint8_t* data(const Entry* e) { return flashBase() + e->offset; }

bool remove(const char* path) {
  if (!find(path)) return false;
  stage();
  uint32_t n = 0;
  for (uint32_t i = 0; i < g_tmp.count; ++i) if (strcmp(g_tmp.e[i].path, path)) g_tmp.e[n++] = g_tmp.e[i];
  g_tmp.count = n;
  return publish();
}

void usage(uint32_t& capacity, uint32_t& used) {
  capacity = kCapacity;
  used = 0;
  for (int i = 0; i < count(); ++i) used += roundUp(at(i)->size);
}

uint32_t largestFree() {
  uint32_t best = 0;
  for (uint32_t need = kCapacity; need >= kBlock; need -= kBlock)
    if (allocate(need, nullptr)) return need;
  return best;
}

uint32_t signature(const char* path, RomSource& src) {
  uint8_t buf[256];
  uint32_t crc = piko::crc32((const uint8_t*)path, strlen(path));
  const uint32_t size = src.size();
  crc ^= piko::crc32((const uint8_t*)&size, 4);
  for (uint32_t off = 0; off < 16384 && off < size; off += sizeof buf) {
    const size_t n = src.read(off, buf, sizeof buf);
    crc = crc * 31 + piko::crc32(buf, n);
  }
  return crc;
}


const char* Writer::begin(const char* path, uint32_t size) {
  active_ = false;
  if (!usable()) return "FIRMWARE OVERLAPS SHELF";
  if (size == 0) return "EMPTY FILE";
  if (strlen(path) >= sizeof(Entry::path)) return "NAME TOO LONG";
  if (size > kCapacity) return "ROM TOO LARGE";
  uint32_t off = allocate(roundUp(size), nullptr);  // keep the old copy until the new one is in
  if (!off && find(path)) {
    remove(path);  // not enough room for both: replace in place
    off = allocate(roundUp(size), nullptr);
  }
  if (!off && count() >= kMaxEntries) return "SHELF FULL (50 ROMS)";
  if (!off) return "NOT ENOUGH SPACE ON SHELF";
  snprintf(path_, sizeof path_, "%s", path);
  offset_ = off;
  size_ = size;
  done_ = 0;
  pageFill_ = 0;
  active_ = true;
  return nullptr;
}

bool Writer::flushPage() {
  const uint32_t pos = offset_ + done_ - pageFill_;  // flash offset of this page
  if ((pos & (kBlock - 1)) == 0) flashErase(pos, kBlock);  // entering a new block
  if (pageFill_ < 256) memset(page_ + pageFill_, 0xFF, 256 - pageFill_);
  flashProgram(pos, page_, 256);
  const bool ok = !memcmp(flashBase() + pos, page_, pageFill_);  // verify
  pageFill_ = 0;
  if (!ok) active_ = false;
  return ok;
}

bool Writer::write(const uint8_t* d, size_t n) {
  if (!active_ || done_ + n > size_) return false;
  while (n) {
    const size_t take = 256 - pageFill_ < n ? 256 - pageFill_ : n;
    memcpy(page_ + pageFill_, d, take);
    pageFill_ += (uint32_t)take;
    done_ += (uint32_t)take;
    d += take;
    n -= take;
    if (pageFill_ == 256 && !flushPage()) return false;
  }
  return true;
}

bool Writer::commit() {
  if (!active_ || done_ != size_) { active_ = false; return false; }
  if (pageFill_ && !flushPage()) return false;
  active_ = false;
  // identity from what is now in flash
  struct Mem : RomSource {
    const uint8_t* p; uint32_t n;
    uint32_t size() const override { return n; }
    size_t read(uint32_t o, void* dst, size_t len) override {
      if (o >= n) return 0;
      if (len > n - o) len = n - o;
      memcpy(dst, p + o, len);
      return len;
    }
  } mem;
  mem.p = flashBase() + offset_;
  mem.n = size_;
  stage();
  uint32_t k = 0;
  for (uint32_t i = 0; i < g_tmp.count; ++i) if (strcmp(g_tmp.e[i].path, path_)) g_tmp.e[k++] = g_tmp.e[i];
  if (k >= (uint32_t)kMaxEntries) return false;
  Entry& e = g_tmp.e[k++];
  memset(&e, 0, sizeof e);
  e.offset = offset_;
  e.size = size_;
  e.sig = signature(path_, mem);
  snprintf(e.path, sizeof e.path, "%s", path_);
  g_tmp.count = k;
  return publish();
}

const Entry* install(const char* path, RomSource& src, void (*progress)(uint32_t, uint32_t, void*), void* ctx) {
  Writer w;
  if (w.begin(path, src.size())) return nullptr;
  uint8_t buf[256];
  for (uint32_t off = 0; off < src.size(); off += sizeof buf) {
    const size_t n = src.read(off, buf, sizeof buf);
    if (n == 0 || !w.write(buf, n)) return nullptr;
    if (progress && (off & 0xFFFF) == 0) progress(off, src.size(), ctx);
  }
  if (!w.commit()) return nullptr;
  if (progress) progress(src.size(), src.size(), ctx);
  return find(path);
}

}  // namespace shelf
