#pragma once
#include "display/display.h"
#include "display/renderer.h"
#include "emulation/content.h"
#include "emulation/gbc_player.h"
#include "emulation/nes_player.h"
#include "emulation/atari_player.h"
#include "input/input.h"
#include "link/link.h"
#include "native/arcade.h"
#include "native/cat.h"
#include "storage/storage.h"
#include "ui/launcher.h"
#include "ui/pad_screen.h"
#include "usbpad/usbpad.h"

// Owns everything and runs the main loop: the launcher, the built-in games and the emulators.
class Runtime {
 public:
  enum class Mode : uint8_t { Idle, Arcade, Neko, Gb, Nes, Atari, Gamepad };
  Runtime() : renderer_(display_), arcade_(renderer_), gb_(renderer_), nes_(renderer_), atari_(renderer_), padView_(renderer_), ui_(renderer_) {}

  void begin();
  void loop();                     // one iteration; call forever
  void core1();                    // second core: presents Game Boy frames
  bool launch(const char* path);   // open -> detect -> native or emulator; false sets lastError()
  void stop();
  void loadSettings();             // /saves/system.sav -> theme, GB SCREEN, high scores (boot)

  Mode mode() const { return mode_; }
  const char* lastError() const { return lastError_; }  // short, launcher-sized; "" when none
  const Arcade& arcade() const { return arcade_; }
  const CatPet& neko() const { return neko_; }
  const GbcPlayer& gb() const { return gb_; }
  const NesPlayer& nes() const { return nes_; }
  const AtariPlayer& atari() const { return atari_; }
  const UsbPad& pad() const { return pad_; }
  const LauncherUI& launcher() const { return ui_; }
  Input& input() { return input_; }

 private:
  bool startContent(ContentType t);
  void fail(const char* why);
  static void sleepUntil(uint64_t dueUs);

  Display display_;
  Renderer renderer_;
  Input input_;
  Link link_;
  Storage storage_;
  FsRomSource rom_;
  Arcade arcade_;
  CatPet neko_;
  GbcPlayer gb_;
  NesPlayer nes_;
  AtariPlayer atari_;
  bool startAtari();
  bool startNes(const char* path);
  void saveNes();
  const uint8_t* installRom(const char* path);  // shelf pointer (copies a plain file onto the shelf); nullptr on failure
  char gbPath_[64] = {};
  ContentType gbType_ = ContentType::GB;
  uint64_t gbDirtySinceUs_ = 0;
  bool startGb(const char* path, ContentType type);
  void saveGb();
  void drawInstall(uint32_t done, uint32_t total);
  uint64_t nekoSavedUs_ = 0;
  UsbPad pad_;
  PadScreen padView_;
  // Leaving a game: SELECT+START held for kMenuHoldUs (a tap is game input).
  static constexpr uint64_t kMenuHoldUs = 1000000;
  uint64_t menuHoldUs_ = 0;
  bool menuHold_ = false;
  void startGamepad();
  void startNeko();
  void saveNeko();
  // Settings and arcade high scores: /saves/system.sav, written after a change settles
  // (1 s, menu only) or before a game starts.
  struct SystemSave {  // "PSY2"; "PSY1" files (theme, gbScale, 2 spare, best[4]) still load
    char magic[4];
    uint8_t theme, gbScale, gbPalette, menuSpeed, hideEmpty, padStick, reserved[2];
    int32_t best[4];  // Arcade::Game order. More games need a new format version.
  };
  void applySettings();  // settings that live outside the launcher (input repeat, arcade bests)
  SystemSave settingsNow() const;
  void saveSettings();
  void settingsTick(uint64_t now);
  SystemSave settingsSaved_{};
  uint64_t settingsChangedUs_ = 0;
  Mode mode_ = Mode::Idle;
  uint64_t nativeDueUs_ = 0;
  uint16_t accPressed_ = 0, accReleased_ = 0, accRepeated_ = 0;  // edges seen since the last native frame ran
  LauncherUI ui_;
  const char* lastError_ = "";
  const char* currentPath_ = "";
};
