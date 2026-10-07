#pragma once
#include <stdint.h>
#include "gfx/canvas.h"
#include "input/input.h"

// NEKO: a virtual pet cat. Feed, pet, play, bath, sleep. Everything is shown through the cat and
// its room (no hands). Needs decay while you play and are saved to flash by the runtime.
class CatPet {
 public:
  enum class Action : uint8_t { None, Feed, Pet, Play, Bath, Sleep };
  static constexpr int kActions = 5;

  struct Save {           // stored in /saves/cat.sav
    uint32_t magic;       // 'KIT1'
    uint8_t hunger, happy, energy, clean;  // 0..100, 100 = fully satisfied
    uint32_t ageMinutes;
  };

  void start(uint32_t seed, const Save* saved);
  void step(const InputState& in);
  void draw(Canvas& c) const;
  Save save() const;

  // State.
  int hunger() const { return hunger_; }
  int happy() const { return happy_; }
  int energy() const { return energy_; }
  int clean() const { return clean_; }
  Action action() const { return action_; }
  bool sleeping() const { return action_ == Action::Sleep; }
  bool curled() const { return pose() == Pose::Curl; }
  bool walking() const { return pose() == Pose::Walk; }
  int menu() const { return menu_; }
  int x() const { return x_; }
  const char* bubble() const { return bubbleText_; }

 private:
  uint32_t rnd();
  void begin(Action a);
  void decay();
  void say(const char* s, int frames);
  void drawRoom(Canvas& c) const;
  enum class Pose : uint8_t { Sit, Walk, Curl };
  Pose pose() const;
  int poseW() const;
  int poseTop() const;  // screen y of the top of the cat in its current pose
  void drawCat(Canvas& c) const;
  void drawHud(Canvas& c) const;
  void drawMenu(Canvas& c) const;

  uint32_t rng_ = 1;
  uint32_t t_ = 0;             // frames since start
  int hunger_ = 80, happy_ = 80, energy_ = 80, clean_ = 80;
  uint32_t ageFrames_ = 0, ageMinutes_ = 0;
  Action action_ = Action::None;
  int actionT_ = 0;            // frames into the current action
  int menu_ = 0;
  int x_ = 80, targetX_ = 80;  // cat position (left edge of the sprite, px)
  int y_ = 0;                  // jump offset (px, up is negative)
  int facing_ = 1;             // +1 right, -1 left (walking sprite)
  int walkT_ = 0, idleT_ = 0;
  int ballX_ = 0, ballV_ = 0;  // yarn
  const char* bubbleText_ = "";
  int bubbleT_ = 0;
};
