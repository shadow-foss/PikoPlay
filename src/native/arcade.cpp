#include "native/arcade.h"
#include <stdio.h>
#include <string.h>
#include "gfx/canvas.h"
#include "gfx/theme.h"

namespace {
const uint16_t kCoral = rgb565(255, 105, 120);
const uint16_t kAmber = rgb565(255, 184, 64);
const uint16_t kTeal = rgb565(64, 200, 170);
const uint16_t kBlue = rgb565(90, 150, 255);
const uint16_t kGreen = rgb565(110, 210, 90);
constexpr int W = Display::W, H = Display::H;
constexpr int kTop = 14;  // HUD height; the playfield is y 14..175

// Snake grid: 27 x 20 cells of 8 px.
constexpr int kCell = 8, kCols = 27, kRows = 20, kGridX = 2, kGridY = 16;
// Flappy
constexpr int kBirdX = 50, kBirdW = 10, kBirdH = 8, kPipeW = 24, kGap = 58, kGround = 166;
constexpr int kPipeSpacing = 92;
constexpr int kFlap = -60;  // Flappy: upward speed after a flap, 1/16 px per frame
// Breakout
constexpr int kPadY = 164, kPadW = 32, kBrickW = 20, kBrickH = 8, kBrickTop = 22;
const uint16_t kBrickColor[5] = {kCoral, kAmber, kGreen, kTeal, kBlue};

inline int clampi(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }
inline int absi(int v) { return v < 0 ? -v : v; }
}  // namespace

const char* Arcade::name(Game g) {
  switch (g) {
    case Game::Pong: return "PONG";
    case Game::Snake: return "SNAKE";
    case Game::Flappy: return "FLAPPY";
    default: return "BREAKOUT";
  }
}
const char* Arcade::blurb(Game g) {
  switch (g) {
    case Game::Pong: return "FIRST TO 7 WINS";
    case Game::Snake: return "EAT, GROW, DON'T CRASH";
    case Game::Flappy: return "A TO FLAP";
    default: return "BREAK EVERY BRICK";
  }
}

uint32_t Arcade::rnd() {
  rng_ ^= rng_ << 13; rng_ ^= rng_ >> 17; rng_ ^= rng_ << 5;
  return rng_;
}

void Arcade::start(Game g, uint32_t seed) {
  game_ = g;
  rng_ = seed ? seed : 0x2545F491u;
  r_.invalidateDiff();
  reset();
  phase_ = Phase::Ready;
  draw();
}

void Arcade::reset() {
  score_ = 0;
  switch (game_) {
    case Game::Pong:
      pong = {};
      pong.py = pong.cy = kTop + (H - kTop) / 2 - 14;
      pong.bx = (W / 2) * 16;
      pong.by = (kTop + (H - kTop) / 2) * 16;
      pong.vx = (rnd() & 1) ? 32 : -32;
      pong.vy = (int)(rnd() % 33) - 16;
      break;
    case Game::Snake:
      memset(&snake, 0, sizeof snake);
      snake.len = 3;
      snake.head = 2;
      for (int i = 0; i < 3; ++i) { snake.x[i] = (uint8_t)(6 + i); snake.y[i] = kRows / 2; }
      snake.dir = snake.next = 0;
      snake.period = 8;
      placeFood();
      break;
    case Game::Flappy:
      flappy = {};
      flappy.y = 70 * 16;
      for (int i = 0; i < 3; ++i) {
        flappy.pipe_x[i] = (W + 30 + i * kPipeSpacing) * 16;
        flappy.gap_y[i] = kTop + 12 + (int)(rnd() % (kGround - kTop - kGap - 24));
      }
      break;
    case Game::Breakout:
      memset(&brk, 0, sizeof brk);
      brk.px = (W - kPadW) / 2;
      brk.lives = 3;
      brk.stuck = true;
      for (auto& row : brk.bricks) for (auto& b : row) b = 1;
      brk.left = 50;
      break;
  }
}

void Arcade::placeFood() {
  for (int tries = 0; tries < 200; ++tries) {
    const int fx = (int)(rnd() % kCols), fy = (int)(rnd() % kRows);
    bool clash = false;
    for (int i = 0; i < snake.len && !clash; ++i) {
      const int k = (snake.head - i + 512) % 512;
      clash = snake.x[k] == fx && snake.y[k] == fy;
    }
    if (!clash) { snake.food_x = fx; snake.food_y = fy; return; }
  }
}

void Arcade::over() {
  phase_ = Phase::Over;
  overT_ = 0;
  const bool versus = game_ == Game::Pong && pongTwo_;  // beating a friend is not a high score
  if (!versus && score_ > best_[(int)game_]) best_[(int)game_] = score_;
}

void Arcade::frame(const InputState& in) {
  const bool go = in.pressed & (BTN_A | BTN_START);
  switch (phase_) {
    case Phase::Ready:
      if (game_ == Game::Pong && (in.pressed & (BTN_UP | BTN_DOWN | BTN_LEFT | BTN_RIGHT))) pongTwo_ = !pongTwo_;
      else if (go || (game_ == Game::Flappy && (in.pressed & BTN_UP))) {
        phase_ = Phase::Playing;
        if (game_ == Game::Flappy) flappy.vy = kFlap;  // the first press is already a flap
      }
      break;
    case Phase::Over:
      if (++overT_ > 45 && go) { reset(); phase_ = Phase::Playing; }
      break;
    case Phase::Paused:
      if (in.pressed & BTN_START) phase_ = Phase::Playing;
      break;
    case Phase::Playing:
      if (in.pressed & BTN_START) { phase_ = Phase::Paused; break; }
      switch (game_) {
        case Game::Pong: stepPong(in); break;
        case Game::Snake: stepSnake(in); break;
        case Game::Flappy: stepFlappy(in); break;
        case Game::Breakout: stepBreakout(in); break;
      }
      break;
  }
  draw();
}


void Arcade::stepPong(const InputState& in) {
  Pong& p = pong;
  constexpr int kPadH = 28, kPadW = 4, kBall = 4, kPlayerX = 6, kCpuX = W - 10;
  if (in.held & BTN_UP) p.py -= 3;
  if (in.held & BTN_DOWN) p.py += 3;
  p.py = clampi(p.py, kTop + 1, H - kPadH - 1);
  if (pongTwo_) {  // player 2 on the face buttons: Y (top) up, A (bottom) down
    if (in.held & BTN_Y) p.cy -= 3;
    if (in.held & BTN_A) p.cy += 3;
  } else {  // CPU tracks the ball, a bit slower than the ball can move, so it can be beaten
    const int target = p.by / 16 + kBall / 2 - kPadH / 2;
    const int speed = 2;
    if (p.vx > 0) p.cy += clampi(target - p.cy, -speed, speed);
  }
  p.cy = clampi(p.cy, kTop + 1, H - kPadH - 1);

  p.bx += p.vx;
  p.by += p.vy;
  int by = p.by / 16;
  if (by <= kTop + 1) { p.by = (kTop + 1) * 16; p.vy = absi(p.vy); }
  if (by >= H - kBall - 1) { p.by = (H - kBall - 1) * 16; p.vy = -absi(p.vy); }
  by = p.by / 16;
  const int bx = p.bx / 16;

  auto bounce = [&](int padY, int dir) {
    const int off = (by + kBall / 2) - (padY + kPadH / 2);  // -16..16
    const int sp = absi(p.vx) + 2 > 80 ? 80 : absi(p.vx) + 2;
    p.vx = dir * sp;
    p.vy = off * 3;
  };
  if (p.vx < 0 && bx <= kPlayerX + kPadW && bx >= kPlayerX - 2 && by + kBall >= p.py && by <= p.py + kPadH) {
    p.bx = (kPlayerX + kPadW) * 16;
    bounce(p.py, 1);
  }
  if (p.vx > 0 && bx + kBall >= kCpuX && bx <= kCpuX + 2 && by + kBall >= p.cy && by <= p.cy + kPadH) {
    p.bx = (kCpuX - kBall) * 16;
    bounce(p.cy, -1);
  }
  if (bx < -kBall || bx > W) {
    if (bx > W) ++score_; else ++p.cpu;
    p.bx = (W / 2) * 16;
    p.by = (kTop + (H - kTop) / 2) * 16;
    p.vx = bx > W ? -32 : 32;  // serve towards whoever lost the point
    p.vy = (int)(rnd() % 33) - 16;
    if (score_ >= 7 || p.cpu >= 7) over();
  }
}


void Arcade::stepSnake(const InputState& in) {
  Snake& s = snake;
  int want = -1;
  if (in.pressed & BTN_RIGHT) want = 0;
  if (in.pressed & BTN_DOWN) want = 1;
  if (in.pressed & BTN_LEFT) want = 2;
  if (in.pressed & BTN_UP) want = 3;
  if (want >= 0 && want != (s.dir + 2) % 4) s.next = want;  // no reversing into yourself
  if (++s.tick < s.period) return;
  s.tick = 0;
  s.dir = s.next;
  static const int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};
  const int nx = s.x[s.head] + dx[s.dir], ny = s.y[s.head] + dy[s.dir];
  if (nx < 0 || ny < 0 || nx >= kCols || ny >= kRows) { over(); return; }
  const bool eat = nx == s.food_x && ny == s.food_y;
  // Collide with the body (the tail cell moves away this step unless we eat).
  for (int i = 0; i < s.len - (eat ? 0 : 1); ++i) {
    const int k = (s.head - i + 512) % 512;
    if (s.x[k] == nx && s.y[k] == ny) { over(); return; }
  }
  s.head = (s.head + 1) % 512;
  s.x[s.head] = (uint8_t)nx;
  s.y[s.head] = (uint8_t)ny;
  if (eat) {
    ++s.len;
    ++score_;
    s.period = s.len < 10 ? 8 : s.len < 20 ? 7 : s.len < 35 ? 6 : 5;
    if (s.len >= kCols * kRows) { over(); return; }
    placeFood();
  }
}


void Arcade::stepFlappy(const InputState& in) {
  Flappy& f = flappy;
  // Units: 1/16 px per frame. Gravity 0.25 px/frame^2, a flap lifts ~28 px, and the first 40
  // frames fall gentler so the first tap after starting is not a race.
  if (in.pressed & (BTN_A | BTN_UP | BTN_B)) f.vy = kFlap;  // flap
  f.vy += f.ticks < 40 ? 3 : 4;                            // gravity
  if (f.vy > 72) f.vy = 72;
  f.y += f.vy;
  ++f.ticks;
  const int y = f.y / 16;
  if (y < kTop) { f.y = kTop * 16; f.vy = 0; }
  if (y + kBirdH >= kGround) { over(); return; }
  for (int i = 0; i < 3; ++i) {
    f.pipe_x[i] -= 32;  // exactly 2 px/frame: whole pixels scroll smoothly (1.5 px judders)
    const int px = f.pipe_x[i] / 16;
    if (px + kPipeW < 0) {  // recycle behind the right-most pipe
      int maxx = 0;
      for (int j = 0; j < 3; ++j) if (f.pipe_x[j] > maxx) maxx = f.pipe_x[j];
      f.pipe_x[i] = maxx + kPipeSpacing * 16;
      f.gap_y[i] = kTop + 12 + (int)(rnd() % (kGround - kTop - kGap - 24));
      f.scored[i] = 0;
      continue;
    }
    const bool overlapX = kBirdX + kBirdW > px && kBirdX < px + kPipeW;
    const int by = f.y / 16;
    if (overlapX && (by < f.gap_y[i] || by + kBirdH > f.gap_y[i] + kGap)) { over(); return; }
    if (!f.scored[i] && px + kPipeW < kBirdX) { f.scored[i] = 1; ++score_; }
  }
}


void Arcade::stepBreakout(const InputState& in) {
  Breakout& b = brk;
  constexpr int kBall = 4;
  if (in.held & BTN_LEFT) b.px -= 4;
  if (in.held & BTN_RIGHT) b.px += 4;
  b.px = clampi(b.px, 0, W - kPadW);
  if (b.stuck) {
    b.bx = (b.px + kPadW / 2 - kBall / 2) * 16;
    b.by = (kPadY - kBall - 1) * 16;
    if (in.pressed & (BTN_A | BTN_UP)) {
      b.stuck = false;
      const int sp = 40 + (50 - b.left) / 5 + score_ / 100;
      b.vx = (rnd() & 1) ? sp / 2 : -sp / 2;
      b.vy = -sp;
    }
    return;
  }
  // Move in two half steps so a fast ball cannot skip through a brick row.
  for (int half = 0; half < 2; ++half) {
    b.bx += b.vx / 2;
    b.by += b.vy / 2;
    int x = b.bx / 16, y = b.by / 16;
    // Keep some sideways speed so the ball never rides a wall.
    const int minVx = (absi(b.vy) + 3) / 4;
    if (x <= 0) { b.bx = 0; b.vx = absi(b.vx) < minVx ? minVx : absi(b.vx); }
    if (x >= W - kBall) { b.bx = (W - kBall) * 16; b.vx = -(absi(b.vx) < minVx ? minVx : absi(b.vx)); }
    if (y <= kTop) { b.by = kTop * 16; b.vy = absi(b.vy); }
    x = b.bx / 16; y = b.by / 16;
    // paddle
    if (b.vy > 0 && y + kBall >= kPadY && y + kBall <= kPadY + 4 && x + kBall >= b.px && x <= b.px + kPadW) {
      const int off = (x + kBall / 2) - (b.px + kPadW / 2);  // -18..18
      const int sp = absi(b.vy) > absi(b.vx) ? absi(b.vy) : absi(b.vx);
      b.vx = off * sp / 20;
      const int minV = sp / 4;  // a centre hit keeps drifting the way it came, never straight up
      if (absi(b.vx) < minV) {
        const bool right = off != 0 ? off > 0 : b.px + kPadW / 2 < W / 2;  // dead centre: away from the near wall
        b.vx = right ? minV : -minV;
      }
      b.vy = -sp;
      b.by = (kPadY - kBall) * 16;
    }
    // bricks: test the ball centre
    const int cx = x + kBall / 2, cy = y + kBall / 2;
    const int col = (cx - 1) / (kBrickW + 2), row = (cy - kBrickTop) / (kBrickH + 2);
    if (cy >= kBrickTop && row >= 0 && row < 5 && col >= 0 && col < 10 && b.bricks[row][col]) {
      b.bricks[row][col] = 0;
      --b.left;
      score_ += 10 * (5 - row);
      b.vy = -b.vy;
      if (b.left == 0) {  // next wave, a bit faster
        for (auto& r : b.bricks) for (auto& k : r) k = 1;
        b.left = 50;
        b.stuck = true;
        return;
      }
      break;
    }
    if (y > H) {
      if (--b.lives <= 0) { over(); return; }
      b.stuck = true;
      return;
    }
  }
}


void Arcade::draw() {
  Canvas c{r_.fb(), W, H};
  const Theme& th = theme();  // games follow the console theme (Options)
  const uint16_t kBg = th.bg, kHud = th.surface, kText = th.text, kMuted = th.muted;
  c.fill(0, 0, W, H, kBg);
  // HUD
  c.fill(0, 0, W, kTop - 1, kHud);
  const uint16_t accent = game_ == Game::Pong ? kTeal : game_ == Game::Snake ? kGreen : game_ == Game::Flappy ? kAmber : kCoral;
  c.text(6, 3, name(game_), accent);
  char b[24];
  if (game_ == Game::Pong) snprintf(b, sizeof b, "%d : %d", score_, pong.cpu);
  else snprintf(b, sizeof b, "%d", score_);
  c.textCenter(W / 2, 3, b, kText);
  if (game_ == Game::Breakout) {
    for (int i = 0; i < brk.lives; ++i) c.fill(W - 10 - i * 8, 5, 5, 4, kCoral);
  } else if (game_ == Game::Pong && pongTwo_) {
    c.textRight(W - 6, 3, "P1 VS P2", kMuted);
  } else {
    snprintf(b, sizeof b, "BEST %d", best_[(int)game_]);
    c.textRight(W - 6, 3, b, kMuted);
  }

  switch (game_) {
    case Game::Pong: {
      for (int y = kTop + 2; y < H; y += 8) c.fill(W / 2 - 1, y, 2, 4, kHud);
      c.fillRound(6, pong.py, 4, 28, 1, kTeal);
      c.fillRound(W - 10, pong.cy, 4, 28, 1, pongTwo_ ? kCoral : kMuted);
      c.fill(pong.bx / 16, pong.by / 16, 4, 4, kText);
      break;
    }
    case Game::Snake: {
      c.frame(kGridX - 1, kGridY - 1, kCols * kCell + 2, kRows * kCell + 2, kHud);
      c.fillRound(kGridX + snake.food_x * kCell + 1, kGridY + snake.food_y * kCell + 1, kCell - 2, kCell - 2, 2, kCoral);
      for (int i = 0; i < snake.len; ++i) {
        const int k = (snake.head - i + 512) % 512;
        c.fill(kGridX + snake.x[k] * kCell + 1, kGridY + snake.y[k] * kCell + 1, kCell - 2, kCell - 2, i == 0 ? kText : kGreen);
      }
      break;
    }
    case Game::Flappy: {
      c.fill(0, kGround, W, H - kGround, th.dark ? rgb565(60, 48, 36) : rgb565(214, 184, 150));
      c.fill(0, kGround, W, 2, kGreen);
      for (int i = 0; i < 3; ++i) {
        const int px = flappy.pipe_x[i] / 16;
        if (px >= W || px + kPipeW < 0) continue;
        c.fill(px, kTop, kPipeW, flappy.gap_y[i] - kTop, kGreen);
        c.fill(px - 2, flappy.gap_y[i] - 6, kPipeW + 4, 6, kGreen);
        const int bot = flappy.gap_y[i] + kGap;
        c.fill(px, bot, kPipeW, kGround - bot, kGreen);
        c.fill(px - 2, bot, kPipeW + 4, 6, kGreen);
      }
      const int by = flappy.y / 16;
      c.fillRound(kBirdX, by, kBirdW, kBirdH, 2, kAmber);
      c.fill(kBirdX + 6, by + 2, 2, 2, kBg);             // eye
      c.fill(kBirdX + kBirdW, by + 4, 3, 2, kCoral);     // beak
      break;
    }
    case Game::Breakout: {
      for (int r = 0; r < 5; ++r)
        for (int col = 0; col < 10; ++col)
          if (brk.bricks[r][col]) c.fillRound(1 + col * (kBrickW + 2), kBrickTop + r * (kBrickH + 2), kBrickW, kBrickH, 1, kBrickColor[r]);
      c.fillRound(brk.px, kPadY, kPadW, 5, 2, kText);
      c.fill(brk.bx / 16, brk.by / 16, 4, 4, kText);
      break;
    }
  }

  // overlays
  if (phase_ == Phase::Ready && game_ == Game::Pong) {  // 1P / 2P choice
    const int bw = 170, bh = 72, bx = (W - bw) / 2, by = 54;
    c.fillRound(bx, by, bw, bh, 3, kHud);
    c.textCenter(W / 2, by + 8, name(game_), accent, 2);
    c.textCenter(W / 2, by + 30, pongTwo_ ? "< 2 PLAYERS >" : "< 1 PLAYER >", kText);
    c.textCenter(W / 2, by + 44, pongTwo_ ? "P1 UP/DOWN  P2 Y/A" : "YOU VS CPU", kMuted);
    c.textCenter(W / 2, by + 58, "A START", kMuted);
  } else if (phase_ != Phase::Playing) {
    const int bw = 150, bh = 46, bx = (W - bw) / 2, by = 64;
    c.fillRound(bx, by, bw, bh, 3, kHud);
    const char* l1 = phase_ == Phase::Ready ? name(game_) : phase_ == Phase::Paused ? "PAUSED" : "GAME OVER";
    if (phase_ == Phase::Over && game_ == Game::Pong)
      l1 = pongTwo_ ? (score_ > pong.cpu ? "P1 WINS" : "P2 WINS") : (score_ > pong.cpu ? "YOU WIN" : "CPU WINS");
    c.textCenter(W / 2, by + 8, l1, accent, 2);
    if (phase_ == Phase::Ready) c.textCenter(W / 2, by + 30, "A START", kMuted);
    else if (phase_ == Phase::Paused) c.textCenter(W / 2, by + 30, "START RESUMES", kMuted);
    else c.textCenter(W / 2, by + 30, "A PLAY AGAIN", kMuted);
  }
  r_.presentDiff();
}
