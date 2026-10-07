#pragma once
#include <FS.h>
#include "emulation/content.h"
#include "storage/rom_source.h"

// RomSource over an fs::File (LittleFS, SD) or over a ROM on the flash shelf (memory-mapped).
class FsRomSource : public RomSource {
 public:
  bool open(fs::FS& fs, const char* path) {
    close();
    f_ = fs.open(path, "r");
    pos_ = 0;
    return (bool)f_;
  }
  void openMem(const uint8_t* p, uint32_t size) {
    close();
    mem_ = p;
    memSize_ = size;
  }
  void close() { if (f_) f_.close(); f_ = fs::File(); pos_ = 0; mem_ = nullptr; memSize_ = 0; }
  uint32_t size() const override { return mem_ ? memSize_ : f_ ? (uint32_t)f_.size() : 0; }
  const uint8_t* map() const override { return mem_; }
  size_t read(uint32_t off, void* dst, size_t len) override {
    if (mem_) {
      if (off >= memSize_) return 0;
      if (len > memSize_ - off) len = memSize_ - off;
      memcpy(dst, mem_ + off, len);
      return len;
    }
    if (!f_) return 0;
    if (off != pos_) { if (!f_.seek(off)) return 0; pos_ = off; }  // avoid redundant seeks
    size_t n = f_.read((uint8_t*)dst, len);
    pos_ += (uint32_t)n;
    return n;
  }
 private:
  fs::File f_;
  uint32_t pos_ = 0;
  const uint8_t* mem_ = nullptr;
  uint32_t memSize_ = 0;
};

// One namespace over the ROM shelf (every /roms/* file, storage/shelf.h), internal LittleFS
// (saves, settings) and optional SD (-DPIKO_SD_CS=<pin>). Paths are backend-agnostic; the shelf wins,
// then SD, then flash.
class Storage {
 public:
  bool begin();
  bool sdPresent() const { return sdOk_; }
  bool openRom(const char* path, FsRomSource& out);
  bool flashPresent() const { return lfsOk_; }
  // Bumped whenever content changes (PikoLink writes/deletes), so listings can be cached.
  uint32_t generation() const { return gen_; }
  void touch() { ++gen_; }
  // Writable backend: the SD card when fitted, otherwise flash (nullptr if neither mounted).
  fs::FS* writeFs();
  // Read side: the backend that holds `path` (SD wins), or nullptr.
  fs::FS* fsHolding(const char* path);
  void flashInfo(uint32_t& total, uint32_t& used);  // shelf + LittleFS

  // Every entry (files and directories) of `dir`, SD first then flash.
  using EntryVisitor = bool (*)(const char* name, uint32_t size, bool isDir, bool onSd, void* ctx);
  void forEachEntry(const char* dir, EntryVisitor cb, void* ctx);

  // Visit regular files in `dir` on SD first, then internal flash. `name` is the bare file name.
  // Return false from the callback to stop. A name present on both backends is reported twice
  // (the SD copy first); callers that list files skip duplicates. Returns files visited.
  using FileVisitor = bool (*)(const char* name, uint32_t size, bool onSd, void* ctx);
  int forEachFile(const char* dir, FileVisitor cb, void* ctx);
  static bool isRomFileName(const char* name);
  int countRoms(const char* dir);

  // /saves/<system>/<game>.sav (game = file name without extension).
  static void savePath(char* out, size_t n, ContentType sys, const char* romPath);

 private:
  bool lfsOk_ = false, sdOk_ = false;
  uint32_t gen_ = 1;
};
