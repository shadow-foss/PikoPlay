#include "storage/storage.h"
#include <LittleFS.h>
#include <Arduino.h>
#include <stdio.h>
#include "storage/shelf.h"
#ifdef PIKO_SD_CS
#include <SD.h>
#endif

bool Storage::begin() {
  lfsOk_ = LittleFS.begin();
  if (lfsOk_) {
    LittleFS.mkdir("/roms");
    static const char* const kRomDirs[] = {"/roms/atari", "/roms/nes", "/roms/gb", "/roms/gbc"};
    for (const char* d : kRomDirs) LittleFS.mkdir(d);  // listed as folders (ROMs live on the shelf)
    LittleFS.mkdir("/saves");
  }
#ifdef PIKO_SD_CS
  // SD shares SPI0 with the display; the display HAL has already routed SCK/MOSI/MISO. SdFat starts at a
  // low clock and only then raises it to SPI_HALF_SPEED.
  sdOk_ = SD.begin(PIKO_SD_CS);
#endif
  return lfsOk_ || sdOk_;
}

// A plain file wins over its shelf copy so a changed file is noticed (Runtime re-installs it);
// ROMs uploaded over PikoLink exist only on the shelf.
bool Storage::openRom(const char* path, FsRomSource& out) {
#ifdef PIKO_SD_CS
  if (sdOk_ && out.open(SDFS, path)) return true;
#endif
  if (lfsOk_ && out.open(LittleFS, path)) return true;
  if (const shelf::Entry* e = shelf::find(path)) {
    out.openMem(shelf::data(e), e->size);
    return true;
  }
  return false;
}

bool Storage::isRomFileName(const char* n) {
  const char* dot = strrchr(n, '.');
  if (!dot) return false;
  const char* exts[] = {".a26", ".a2600", ".nes", ".gb", ".gbc", ".bin"};
  for (const char* e : exts) {
    if (!strcasecmp(dot, e)) return true;
  }
  return false;
}

struct EntryCtx { Storage::EntryVisitor cb; void* ctx; };

// Returns false when the visitor asked to stop.
static bool walkFs(fs::FS& fs, const char* dirPath, bool onSd, Storage::EntryVisitor cb, void* ctx) {
  fs::File dir = fs.open(dirPath, "r");
  if (!dir || !dir.isDirectory()) return true;
  for (;;) {
    fs::File f = dir.openNextFile();
    if (!f) break;
    String name = f.name();
    const bool isDir = f.isDirectory();
    const uint32_t size = isDir ? 0 : (uint32_t)f.size();
    f.close();
    if (!cb(name.c_str(), size, isDir, onSd, ctx)) return false;
  }
  return true;
}

void Storage::forEachEntry(const char* dir, EntryVisitor cb, void* ctx) {
  const size_t dl = strlen(dir);  // shelf ROMs directly inside `dir`
  for (int i = 0; i < shelf::count(); ++i) {
    const shelf::Entry* e = shelf::at(i);
    if (strncmp(e->path, dir, dl) || e->path[dl] != '/' || strchr(e->path + dl + 1, '/')) continue;
    if (!cb(e->path + dl + 1, e->size, false, false, ctx)) return;
  }
#ifdef PIKO_SD_CS
  if (sdOk_ && !walkFs(SDFS, dir, true, cb, ctx)) return;
#endif
  if (lfsOk_) walkFs(LittleFS, dir, false, cb, ctx);
}

int Storage::forEachFile(const char* dir, FileVisitor cb, void* ctx) {
  struct Adapter { FileVisitor cb; void* ctx; int visited; } ad{cb, ctx, 0};
  forEachEntry(dir, [](const char* name, uint32_t size, bool isDir, bool onSd, void* c) {
    Adapter& a = *(Adapter*)c;
    if (isDir) return true;
    ++a.visited;
    return a.cb(name, size, onSd, a.ctx);
  }, &ad);
  return ad.visited;
}

fs::FS* Storage::writeFs() {
#ifdef PIKO_SD_CS
  if (sdOk_) return &SDFS;
#endif
  return lfsOk_ ? &LittleFS : nullptr;
}

fs::FS* Storage::fsHolding(const char* path) {
#ifdef PIKO_SD_CS
  if (sdOk_ && SDFS.exists(path)) return &SDFS;
#endif
  return (lfsOk_ && LittleFS.exists(path)) ? &LittleFS : nullptr;
}

void Storage::flashInfo(uint32_t& total, uint32_t& used) {
  shelf::usage(total, used);
  if (!lfsOk_) return;
  FSInfo info;
  if (LittleFS.info(info)) { total += (uint32_t)info.totalBytes; used += (uint32_t)info.usedBytes; }
}

int Storage::countRoms(const char* dir) {
  int n = 0;
  forEachFile(dir, [](const char* name, uint32_t, bool, void* c) {
    if (isRomFileName(name)) ++*(int*)c;
    return true;
  }, &n);
  return n;
}

static void buildPath(char* out, size_t n, const char* root, ContentType sys, const char* romPath,
                      const char* ext) {
  const char* base = strrchr(romPath, '/');
  base = base ? base + 1 : romPath;
  const char* dot = strrchr(base, '.');
  int len = dot ? (int)(dot - base) : (int)strlen(base);
  snprintf(out, n, "/%s/%s/%.*s.%s", root, contentSystemDir(sys), len, base, ext);
}

void Storage::savePath(char* o, size_t n, ContentType s, const char* r) { buildPath(o, n, "saves", s, r, "sav"); }
