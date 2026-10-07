#include "link/link.h"
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "util/crc32.h"

namespace {
constexpr uint8_t kMagic1 = 0xA5, kMagic2 = 0x5A;
constexpr uint32_t kFrameTimeoutUs = 200000;
enum Cmd : uint8_t { HELLO = 1, LIST, READ, WRITE_BEGIN, WRITE_DATA, WRITE_END, DELETE_, MKDIR, LAUNCH = 0x0B };
enum Err : uint8_t { E_REQ = 1, E_PATH, E_NOTFOUND, E_BUSY, E_STORAGE, E_ORDER, E_CRC, E_NOFS };
const char* kFw = "PIKOPLAY 0.5";

uint32_t rd32(const uint8_t* p) { return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24); }
uint16_t rd16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }
void wr32(uint8_t* p, uint32_t v) { for (int i = 0; i < 4; ++i) p[i] = (uint8_t)(v >> (8 * i)); }

// Reads a `len u8, bytes` string; returns bytes consumed or 0 on error.
size_t getStr(const uint8_t* p, size_t n, char* out, size_t cap) {
  if (n < 1 || (size_t)p[0] + 1 > n || (size_t)p[0] >= cap) return 0;
  memcpy(out, p + 1, p[0]);
  out[p[0]] = 0;
  return (size_t)p[0] + 1;
}
}  // namespace

bool Link::pathOk(const char* p) {
  const size_t n = strlen(p);
  if (n < 2 || n > 63 || p[0] != '/') return false;
  for (size_t k = 0; k < n; ++k) if ((uint8_t)p[k] < 0x20 || (uint8_t)p[k] > 0x7E || p[k] == '\\') return false;
  // Components: none empty (no "//" or trailing '/'), none "." or "..".
  bool first = true, rootOk = false;
  size_t start = 1;
  for (size_t k = 1; k <= n; ++k) {
    if (k < n && p[k] != '/') continue;
    const size_t cl = k - start;
    if (cl == 0) return false;
    if (cl == 1 && p[start] == '.') return false;
    if (cl == 2 && p[start] == '.' && p[start + 1] == '.') return false;
    if (first) {
      static const char* const roots[] = {"roms", "saves"};
      for (const char* r : roots) if (strlen(r) == cl && !strncmp(p + start, r, cl)) rootOk = true;
      first = false;
    }
    start = k + 1;
  }
  return rootOk;
}

void Link::begin() {
  Serial.begin(115200);
}

int Link::sweepTemp(Storage& st) {
  static const char* const dirs[] = {"/roms/atari", "/roms/nes", "/roms/gb", "/roms/gbc", "/saves"};
  int removed = 0;
  for (const char* dir : dirs) {
    struct Found { char name[48]; bool sd; };
    struct Ctx { Found f[8]; int n; } ctx{};
    st.forEachEntry(dir, [](const char* name, uint32_t, bool isDir, bool onSd, void* v) {
      Ctx& c = *(Ctx*)v;
      const size_t l = strlen(name);
      if (isDir || l < 6 || l >= sizeof c.f[0].name || strcmp(name + l - 5, ".part") || c.n >= 8) return true;
      strcpy(c.f[c.n].name, name);
      c.f[c.n++].sd = onSd;
      return true;
    }, &ctx);
    for (int i = 0; i < ctx.n; ++i) {
      char path[96];
      snprintf(path, sizeof path, "%s/%s", dir, ctx.f[i].name);
      fs::FS* f = st.fsHolding(path);
      if (f && f->remove(path)) ++removed;
    }
  }
  return removed;
}

void Link::reply(uint8_t cmd, uint8_t seq, const uint8_t* p, size_t n) {
  uint8_t head[6] = {kMagic1, kMagic2, cmd, seq, (uint8_t)n, (uint8_t)(n >> 8)};
  uint32_t crc = 0xFFFFFFFFu;
  crc = piko::crc32Update(crc, head + 2, 4);
  crc = piko::crc32Update(crc, p, n);
  crc = ~crc;
  uint8_t tail[4];
  wr32(tail, crc);
  Serial.write(head, 6);
  if (n) Serial.write(p, n);
  Serial.write(tail, 4);
}

void Link::error(uint8_t seq, uint8_t code, const char* msg) {
  uint8_t b[40];
  b[0] = code;
  size_t n = strlen(msg);
  if (n > 38) n = 38;
  memcpy(b + 1, msg, n);
  reply(0xFF, seq, b, 1 + n);
}

void Link::abortUpload(Storage&) {
  if (!up_) return;
  if (upShelf_) {
    shelfUp_.abort();  // the space was never indexed: nothing to clean up
  } else {
    upFile_.close();
    if (upFs_) upFs_->remove(upTmp_);
  }
  up_ = false;
}

void Link::poll(uint64_t now, Storage& storage, bool busy) {
  if (state_ != kIdle && now - frameStartUs_ > kFrameTimeoutUs) state_ = kIdle;  // stalled frame
  for (int n = 0; n < 128 && Serial.available() > 0; ++n) feed((uint8_t)Serial.read(), now, storage, busy);
  // A game started mid-upload (someone pressed A on the handheld): uploads are refused while a
  // game runs, so abandon this one now and delete its temp file instead of leaving it behind.
  if (busy && up_) abortUpload(storage);
}

void Link::feed(uint8_t c, uint64_t now, Storage& storage, bool busy) {
  switch (state_) {
    case kIdle:
      if (c == kMagic1) { state_ = kSync2; frameStartUs_ = now; }
      return;
    case kSync2:
      if (c == kMagic2) { state_ = kHead; headN_ = 0; return; }
      state_ = kIdle;
      if (c == kMagic1) { state_ = kSync2; frameStartUs_ = now; return; }
      return;
    case kHead:
      head_[headN_++] = c;
      if (headN_ == 4) {
        len_ = rd16(head_ + 2);
        got_ = 0;
        if (len_ > kMaxPayload) { state_ = kIdle; return; }  // oversize: drop, resync
        state_ = len_ ? kBody : kCrc;
        crcN_ = 0;
      }
      return;
    case kBody:
      buf_[got_++] = c;
      if (got_ == len_) { state_ = kCrc; crcN_ = 0; }
      return;
    case kCrc:
      crc_[crcN_++] = c;
      if (crcN_ == 4) {
        state_ = kIdle;
        uint32_t crc = piko::crc32Update(0xFFFFFFFFu, head_, 4);
        crc = ~piko::crc32Update(crc, buf_, len_);
        if (crc == rd32(crc_)) handle(head_[0], head_[1], buf_, len_, storage, busy);
      }
      return;
  }
}

void Link::handle(uint8_t cmd, uint8_t seq, const uint8_t* p, uint16_t n, Storage& st, bool busy) {
  char path[64];
  switch (cmd) {
    case HELLO: {
      uint8_t b[16 + 24];
      uint32_t total, used;
      st.flashInfo(total, used);
      b[0] = 1;
      b[1] = (uint8_t)((st.sdPresent() ? 1 : 0) | (st.flashPresent() ? 2 : 0) | (busy ? 4 : 0));
      wr32(b + 2, total);
      wr32(b + 6, used);
      b[10] = (uint8_t)strlen(kFw);
      memcpy(b + 11, kFw, b[10]);
      reply(HELLO | 0x80, seq, b, 11 + b[10]);
      return;
    }
    case LIST: {
      if (n < 3 || !getStr(p + 2, n - 2, path, sizeof path)) { error(seq, E_REQ, "bad list"); return; }
      const uint16_t start = rd16(p);
      if (!strcmp(path, "/")) {  // virtual root: the four managed folders
        static const char* const roots[] = {"roms", "saves"};
        uint8_t b[64];
        size_t o = 2;
        for (const char* r : roots) { wr32(b + o, 0); b[o + 4] = 1; b[o + 5] = (uint8_t)strlen(r); memcpy(b + o + 6, r, strlen(r)); o += 6 + strlen(r); }
        b[0] = 4; b[1] = 0;
        reply(LIST | 0x80, seq, b, o);
        return;
      }
      if (!pathOk(path)) { error(seq, E_PATH, "bad path"); return; }
      uint8_t out[512];
      struct C { uint16_t idx, start; uint8_t* out; size_t o; uint8_t count; bool more; } c{0, start, out, 2, 0, false};
      st.forEachEntry(path, [](const char* name, uint32_t size, bool isDir, bool onSd, void* v) {
        C& c = *(C*)v;
        const size_t nl = strlen(name) > 60 ? 60 : strlen(name);
        if (c.idx++ < c.start) return true;
        if (c.o + 6 + nl > 512 || c.count == 255) { c.more = true; return false; }  // listings stay <= 512 B (older clients)
        wr32(c.out + c.o, size);
        c.out[c.o + 4] = (uint8_t)((isDir ? 1 : 0) | (onSd ? 2 : 0));
        c.out[c.o + 5] = (uint8_t)nl;
        memcpy(c.out + c.o + 6, name, nl);
        c.o += 6 + nl;
        ++c.count;
        return true;
      }, &c);
      out[0] = c.count;
      out[1] = c.more ? 1 : 0;
      reply(LIST | 0x80, seq, out, c.o);
      return;
    }
    case READ: {
      if (n < 7 || !getStr(p + 6, n - 6, path, sizeof path)) { error(seq, E_REQ, "bad read"); return; }
      if (!pathOk(path)) { error(seq, E_PATH, "bad path"); return; }
      const uint32_t off = rd32(p);
      uint16_t want = rd16(p + 4);
      if (want > kMaxPayload) want = kMaxPayload;
      if (const shelf::Entry* e = shelf::find(path)) {
        const uint32_t got = off >= e->size ? 0 : (e->size - off < want ? e->size - off : want);
        memcpy(buf_, shelf::data(e) + off, got);
        reply(READ | 0x80, seq, buf_, got);
        return;
      }
      fs::FS* f = st.fsHolding(path);
      if (!f) { error(seq, E_NOTFOUND, "not found"); return; }
      fs::File file = f->open(path, "r");
      if (!file || file.isDirectory()) { error(seq, E_NOTFOUND, "not a file"); return; }
      uint8_t* const out = buf_;  // the request is fully parsed: reuse its buffer for the reply
      size_t got = 0;
      if (file.seek(off)) got = file.read(out, want);
      file.close();
      reply(READ | 0x80, seq, out, got);
      return;
    }
    case LAUNCH: {  // path string: start this game/ROM (leaves whatever is running)
      if (n < 1 || !getStr(p, n, path, sizeof path) || !pathOk(path)) { error(seq, E_PATH, "bad path"); return; }
      snprintf(launch_, sizeof launch_, "%s", path);
      reply(LAUNCH | 0x80, seq, nullptr, 0);
      return;
    }
    default: break;
  }

  // Everything below changes storage.
  if (busy) { error(seq, E_BUSY, "game running"); return; }
  fs::FS* wfs = st.writeFs();
  if (!wfs) { error(seq, E_NOFS, "no storage"); return; }
  switch (cmd) {
    case WRITE_BEGIN: {
      if (n < 6 || !getStr(p + 4, n - 4, path, sizeof path)) { error(seq, E_REQ, "bad begin"); return; }
      if (!pathOk(path)) { error(seq, E_PATH, "bad path"); return; }
      abortUpload(st);
      if (shelf::isShelfPath(path)) {
        if (const char* why = shelfUp_.begin(path, rd32(p))) { error(seq, E_STORAGE, why); return; }
        strcpy(upPath_, path);
        up_ = true; upShelf_ = true; upSize_ = rd32(p); upOff_ = 0; upCrc_ = 0xFFFFFFFFu;
        reply(WRITE_BEGIN | 0x80, seq, nullptr, 0);
        return;
      }
      upShelf_ = false;
      snprintf(upTmp_, sizeof upTmp_, "%s.part", path);
      strcpy(upPath_, path);
      upFs_ = wfs;
      upFile_ = wfs->open(upTmp_, "w");
      if (!upFile_) { error(seq, E_STORAGE, "cannot create"); return; }
      up_ = true; upSize_ = rd32(p); upOff_ = 0; upCrc_ = 0xFFFFFFFFu;
      reply(WRITE_BEGIN | 0x80, seq, nullptr, 0);
      return;
    }
    case WRITE_DATA: {
      if (!up_ || n < 4 || rd32(p) != upOff_ || upOff_ + (n - 4) > upSize_) { error(seq, E_ORDER, "out of order"); return; }
      const bool wrote = upShelf_ ? shelfUp_.write(p + 4, n - 4) : upFile_.write(p + 4, n - 4) == (size_t)(n - 4);
      if (!wrote) { abortUpload(st); error(seq, E_STORAGE, "write failed"); return; }
      upCrc_ = piko::crc32Update(upCrc_, p + 4, n - 4);
      upOff_ += n - 4;
      reply(WRITE_DATA | 0x80, seq, nullptr, 0);
      return;
    }
    case WRITE_END: {
      if (!up_ || n < 4) { error(seq, E_ORDER, "no upload"); return; }
      if (upOff_ != upSize_ || ~upCrc_ != rd32(p)) { abortUpload(st); error(seq, E_CRC, "size/crc mismatch"); return; }
      if (upShelf_) {
        up_ = false;
        if (!shelfUp_.commit()) { error(seq, E_STORAGE, "shelf write failed"); return; }
        st.touch();
        reply(WRITE_END | 0x80, seq, nullptr, 0);
        return;
      }
      upFile_.close();
      upFs_->remove(upPath_);
      const bool ok = upFs_->rename(upTmp_, upPath_);
      up_ = false;
      if (!ok) { upFs_->remove(upTmp_); error(seq, E_STORAGE, "rename failed"); return; }
      st.touch();
      reply(WRITE_END | 0x80, seq, nullptr, 0);
      return;
    }
    case DELETE_: {
      if (n < 1 || !getStr(p, n, path, sizeof path)) { error(seq, E_REQ, "bad delete"); return; }
      if (!pathOk(path) || !strchr(path + 1, '/')) { error(seq, E_PATH, "bad path"); return; }  // never a root folder
      if (shelf::find(path)) {
        if (!shelf::remove(path)) { error(seq, E_STORAGE, "delete failed"); return; }
        st.touch();
        reply(DELETE_ | 0x80, seq, nullptr, 0);
        return;
      }
      fs::FS* f = st.fsHolding(path);
      if (!f) { error(seq, E_NOTFOUND, "not found"); return; }
      if (!f->remove(path)) { error(seq, E_STORAGE, "delete failed"); return; }
      st.touch();
      reply(DELETE_ | 0x80, seq, nullptr, 0);
      return;
    }
    case MKDIR: {
      if (n < 1 || !getStr(p, n, path, sizeof path)) { error(seq, E_REQ, "bad mkdir"); return; }
      if (!pathOk(path)) { error(seq, E_PATH, "bad path"); return; }
      if (!wfs->mkdir(path)) { error(seq, E_STORAGE, "mkdir failed"); return; }
      st.touch();
      reply(MKDIR | 0x80, seq, nullptr, 0);
      return;
    }
    default: error(seq, E_REQ, "unknown command");
  }
}
