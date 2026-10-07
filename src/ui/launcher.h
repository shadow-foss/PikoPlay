#pragma once
#include <stddef.h>
#include <stdint.h>
#include "display/renderer.h"
#include "input/input.h"
#include "native/arcade.h"
#include "storage/storage.h"

// What the launcher asks the runtime to do. The launcher never starts anything itself.
struct LauncherAction {
  enum class Kind : uint8_t { None, PlayArcade, PlayNeko, PlayRom, ClearScores, Gamepad };
  Kind kind = Kind::None;
  uint8_t arcade = 0;  // PlayArcade: Arcade::Game index
  char path[64] = {};  // PlayRom: absolute path, e.g. /roms/gb/game.gb
};

// Handheld launcher, laid out like a RetroPie front end: a carousel of systems (HOME), then a
// game list per system (left/right in a list jumps to the neighbouring system), GAMEPAD (the
// console as a USB gamepad for a PC) and SETTINGS.
// Look: brutalist. Flat fills, 2 px rules, an inverted bar for the selection, one accent colour.
// Each change re-composes the whole page into the (idle) framebuffer and
// Renderer::presentDiff() sends only the pixels that differ from the panel.
// All controls come from the shared logical InputState; it never reads serial or GPIO.
class LauncherUI {
 public:
  enum class Page : uint8_t { Home, Games, Roms, Settings };
  // Carousel entries, in carousel order.
  enum class System : uint8_t { Arcade, Neko, Atari, Nes, Gb, Gbc, Gamepad, Settings };
  static constexpr int kSystemCount = 8;
  // Settings rows. The first ones change something; the rest are read-only facts.
  enum Setting : uint8_t {
    kSetTheme, kSetGbScreen, kSetGbPalette, kSetMenuSpeed, kSetEmpty, kSetPadDpad, kSetScores,
    kInfoStorage, kInfoBrightness, kInfoBattery, kInfoFirmware, kSettingCount
  };

  explicit LauncherUI(Renderer& r) : r_(r) {}
  void begin() { open(); }
  void open();
  bool active() const { return active_; }
  void close() { active_ = false; }
  // Bit i set = a real emulator core is linked for system i (Atari 2600, NES, GB, GBC).
  void setEmulatorCores(uint8_t mask) { coreMask_ = mask; }
  // Arcade high scores, shown next to each game (Arcade::Game order).
  void setArcadeBest(const int best[Arcade::kGameCount]) {
    for (int i = 0; i < Arcade::kGameCount; ++i) if (best_[i] != best[i]) { best_[i] = best[i]; dirty_ = true; }
  }
  // Transient one-line notice in the footer (launch failures, unavailable features).
  void notify(const char* msg, uint64_t nowUs);
  LauncherAction frame(const InputState& in, uint64_t nowUs, Storage& storage);

  // Current state.
  Page page() const { return page_; }
  int homeSelection() const { return home_; }  // index into the visible carousel
  int carouselCount() const { return nCarousel_; }
  System homeSystem() const { return carousel_[home_]; }
  System listSystem() const { return listSys_; }
  bool animating() const { return false; }
  int gameCount() const { return nGames_; }
  const char* gameTitle(int i) const { return games_[i].title; }
  const char* gameDetail(int i) const { return games_[i].detail; }
  int gameSelection() const { return sel_[1]; }
  int romCount() const { return nRoms_; }
  int romSelection() const { return sel_[2]; }
  int settingSelection() const { return sel_[3]; }
  int theme() const;
  const char* toast() const { return toastText_; }
  uint32_t lastPixelsSent() const { return lastSent_; }

 private:
  struct Game {
    char title[17];
    char detail[24];  // one-line blurb
    int8_t arcade;    // index into Arcade::Game
  };
  struct Row {
    const char* title;
    const char* tag;
    bool dim;   // unavailable / refused: drawn muted, tag in the warning colour
    bool knob;  // settings value that LEFT/RIGHT/A change: drawn as "< VALUE >" when selected
  };
  static constexpr int kMaxGames = Arcade::kGameCount;
  static constexpr int kVisibleRows = 9;

  void render();
  void drawHome();
  void drawHeader(const char* title, int index, int count);
  void drawList(int count, Row (LauncherUI::*row)(int) const);
  void drawFooter();
  Row gameRow(int i) const;
  Row romRow(int i) const;
  Row settingRow(int i) const;
  const char* detailLine() const;
  void scanGames();
  void scanRoms(Storage& storage);
  void countRoms(Storage& storage);
  void rebuildCarousel();
  bool isList(System s) const;
  bool hasCore(System s) const;
  int romCountOf(System s) const;
  void enter(System s, Storage& storage, uint64_t nowUs);
  void go(Page p, Storage& storage);
  void move(int dir, int count);
  void change(int dir, uint64_t nowUs, LauncherAction& act);

  Renderer& r_;
  bool active_ = true;
  bool dirty_ = true;
  Page page_ = Page::Home;
  uint8_t coreMask_ = 0;
  System carousel_[kSystemCount] = {};
  int nCarousel_ = 0;
  int home_ = 0;
  System listSys_ = System::Arcade;           // system shown by the Games / Roms page
  int sel_[4] = {}, top_[4] = {};             // per-page selection and scroll (index = Page)
  Game games_[kMaxGames] = {};
  int nGames_ = 0;
  int romCount_[4] = {};                      // per emulator system (Atari, NES, GB, GBC)
  static constexpr int kMaxRoms = 24;
  struct Rom { char name[40]; uint32_t size; };
  Rom roms_[kMaxRoms] = {};
  int nRoms_ = 0;
  int best_[Arcade::kGameCount] = {};
  bool sdOk_ = false, flashOk_ = false;
  uint32_t flashUsed_ = 0, flashTotal_ = 0;
  char toastText_[40] = {};
  uint64_t toastUntilUs_ = 0;
  uint64_t clearArmedUntilUs_ = 0;            // HIGH SCORES: second A within 3 s clears them
  uint32_t lastSent_ = 0;
  uint32_t gamesGen_ = 0, countGen_ = 0;      // storage generation the cached listings belong to
  mutable char scratch_[3][40] = {};
};
