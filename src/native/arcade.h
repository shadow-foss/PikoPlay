#pragma once
#include <stdint.h>
#include "display/renderer.h"
#include "input/input.h"

// Built-in arcade games: Pong, Snake, Flappy, Breakout. Each frame the active game draws the whole
// screen into the framebuffer and Renderer::presentDiff() sends only what changed.
// Rules run on a fixed 60 Hz step (one frame() call), so they are deterministic per seed.
class Arcade {
 public:
  enum class Game : uint8_t { Pong, Snake, Flappy, Breakout };
  enum class Phase : uint8_t { Ready, Playing, Paused, Over };
  static constexpr int kGameCount = 4;
  static const char* name(Game g);
  static const char* blurb(Game g);

  explicit Arcade(Renderer& r) : r_(r) {}
  void start(Game g, uint32_t seed);
  void frame(const InputState& in);

  // State.
  Game game() const { return game_; }
  Phase phase() const { return phase_; }
  int score() const { return score_; }
  int best(Game g) const { return best_[(int)g]; }
  bool pongTwoPlayers() const { return pongTwo_; }
  void setBest(Game g, int v) { best_[(int)g] = v < 0 ? 0 : v; }  // restored from /saves/system.sav

  // Per-game state.
  struct Pong { int py, cy, bx, by, vx, vy, cpu; };                 // 1/16 px fixed point for ball
  struct Snake { uint8_t x[512], y[512]; int len, head, dir, next, food_x, food_y, tick, period; };
  struct Flappy { int y, vy, pipe_x[3], gap_y[3], scored[3], ticks; };  // y, vy, pipe_x in 1/16 px
  struct Breakout { int px, bx, by, vx, vy, lives; bool stuck; uint8_t bricks[5][10]; int left; };
  Pong pong{};
  Snake snake{};
  Flappy flappy{};
  Breakout brk{};

 private:
  uint32_t rnd();
  void reset();
  void over();
  void stepPong(const InputState& in);
  void stepSnake(const InputState& in);
  void stepFlappy(const InputState& in);
  void stepBreakout(const InputState& in);
  void draw();
  void placeFood();

  Renderer& r_;
  Game game_ = Game::Pong;
  Phase phase_ = Phase::Ready;
  int score_ = 0;
  int best_[kGameCount] = {};
  bool pongTwo_ = false;  // Pong 2P: right paddle is player 2 (Y up, A down); chosen on the start screen
  int overT_ = 0;         // frames since GAME OVER (a held button must not restart at once)
  uint32_t rng_ = 1;
};
