#include "native/cat.h"
#include <string.h>
#include "gfx/theme.h"

namespace {
constexpr int W = Display::W, H = Display::H;
constexpr int kScale = 3;
constexpr int kCatW = 16 * kScale, kCatH = 16 * kScale;
constexpr int kFloorY = 124;           // where the wall meets the floor
constexpr int kGroundY = 152;          // the line everything stands on (paws, bowl, cushion, yarn)
constexpr int kCatY = kGroundY - kCatH; // top of the sitting cat
constexpr int kBowlX = 22, kMinX = 8, kMaxX = W - kCatW - 20;
constexpr int kCushionX = W - 72;      // cat sleeps here (left edge of the curled sprite), under the window

// Front-facing sitting cat, 16x16. o outline, f fur, w white, p pink, e eye, h eye highlight.
const char* const kCat[16] = {
  "..o..........o..",
  "..oo........oo..",
  "..opo......opo..",
  "..offooooooffo..",
  ".offffffffffffo.",
  ".offheffffheffo.",
  ".offeeffffeeffo.",
  ".ofwwwwppwwwwfo.",
  ".offwwowwowwffo.",
  "..offwwwwwwffo..",
  "...oooffffooo...",
  "..offfwwwwfffo..",
  ".offffwwwwffffo.",
  ".offffwwwwffffo.",
  ".ofwwwffffwwwfo.",
  "..oooooooooooo..",
};
// Tail, 5x8, drawn behind the body at the right; two swish frames.
const char* const kTail[2][8] = {
  {"...oo", "..ofo", "..ofo", ".ofo.", ".ofo.", "ofo..", "of...", "o...."},
  {".....", ".....", "...oo", "..ofo", ".ofo.", "ofo..", "of...", "o...."},
};

// Walking, side view facing right, 20x12; two frames (stride, passing).
const char* const kWalk[2][12] = {
  {"..............o...o.",
   ".............ofo.ofo",
   ".oo..........offfffo",
   "of.o.........offfefo",
   "of..ooooooooooffwwpo",
   ".of.offffffffffwwwo.",
   "..oofffffffffffffo..",
   "....offffffffffffo..",
   "....offwwwwwwwwffo..",
   "...ofo.ooooooo.ofo..",
   "..ofo...........ofo.",
   "..oo.............oo."},
  {"..............o...o.",
   ".............ofo.ofo",
   ".oo..........offfffo",
   "of.o.........offfefo",
   "of..ooooooooooffwwpo",
   ".of.offffffffffwwwo.",
   "..oofffffffffffffo..",
   "....offffffffffffo..",
   "....offwwwwwwwwffo..",
   ".....ofooooooofo....",
   ".....ofo.....ofo....",
   ".....oo......oo....."},
};
// Curled up asleep on the cushion, 20x10, head on the right, tail wrapped round the front.
const char* const kCurl[10] = {
  "..............o..o..",
  "......oooooooofoofo.",
  "....ooffffffffoffffo",
  "...offfffffffffffffo",
  "..offffffffffffooofo",
  "..offfffffffffffwwpo",
  ".offffffffffffffwwo.",
  ".offoooooooooooooffo",
  ".ofwwffffffffffffffo",
  "..oooooooooooooooo..",
};

const uint16_t kOutline = rgb565(70, 42, 30);
const uint16_t kFur = rgb565(255, 168, 86);
const uint16_t kWhite = rgb565(252, 246, 236);
const uint16_t kPink = rgb565(255, 140, 165);
const uint16_t kEye = rgb565(34, 30, 44);
const uint16_t kDirt = rgb565(140, 100, 60);
const uint16_t kWater = rgb565(110, 180, 255);

inline int clampi(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }

// 7x7 icons (row bits, MSB left): fish, heart, bolt, drop, moon.
const uint8_t kIcons[5][7] = {
  {0x00, 0x1C, 0x3E, 0x7F, 0x3E, 0x1C, 0x00},  // fish-ish (food)
  {0x36, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C, 0x08},  // heart (happy / pet)
  {0x0C, 0x18, 0x3E, 0x0C, 0x18, 0x10, 0x00},  // bolt (energy / play)
  {0x08, 0x1C, 0x1C, 0x3E, 0x3E, 0x3E, 0x1C},  // drop (clean / bath)
  {0x1C, 0x30, 0x60, 0x60, 0x60, 0x30, 0x1C},  // moon (sleep)
};
const char* const kActionName[5] = {"FEED", "PET", "PLAY", "BATH", "SLEEP"};
const uint16_t kActionColor[5] = {accent::amber, accent::coral, accent::green, accent::blue, accent::teal};

void icon(Canvas& c, int x, int y, int i, uint16_t col) {
  for (int r = 0; r < 7; ++r)
    for (int b = 0; b < 7; ++b)
      if (kIcons[i][r] & (0x40 >> b)) c.dot(x + b, y + r, col);
}
}  // namespace

uint32_t CatPet::rnd() {
  rng_ ^= rng_ << 13; rng_ ^= rng_ >> 17; rng_ ^= rng_ << 5;
  return rng_;
}

void CatPet::start(uint32_t seed, const Save* s) {
  rng_ = seed ? seed : 7;
  t_ = 0;
  action_ = Action::None;
  actionT_ = 0;
  menu_ = 0;
  x_ = targetX_ = (W - kCatW) / 2;
  y_ = 0;
  bubbleT_ = 0;
  if (s && s->magic == 0x3154494Bu) {
    hunger_ = clampi(s->hunger, 0, 100); happy_ = clampi(s->happy, 0, 100);
    energy_ = clampi(s->energy, 0, 100); clean_ = clampi(s->clean, 0, 100);
    ageMinutes_ = s->ageMinutes;
    say("MEOW!", 90);
  } else {
    hunger_ = happy_ = energy_ = clean_ = 80;
    ageMinutes_ = 0;
    say("HI!", 90);
  }
}

CatPet::Save CatPet::save() const {
  Save s{};
  s.magic = 0x3154494Bu;  // "KIT1"
  s.hunger = (uint8_t)hunger_; s.happy = (uint8_t)happy_; s.energy = (uint8_t)energy_; s.clean = (uint8_t)clean_;
  s.ageMinutes = ageMinutes_;
  return s;
}

void CatPet::say(const char* s, int frames) {
  bubbleText_ = s;
  bubbleT_ = frames;
}

// Needs go down slowly while you play (a full bar lasts roughly 10-20 minutes).
void CatPet::decay() {
  if (++ageFrames_ >= 3600) { ageFrames_ = 0; ++ageMinutes_; }
  if (t_ % 480 == 0 && hunger_ > 0) --hunger_;
  if (t_ % 600 == 0 && happy_ > 0) --happy_;
  if (t_ % 900 == 0 && clean_ > 0) --clean_;
  if (action_ != Action::Sleep && t_ % 720 == 0 && energy_ > 0) --energy_;
  if (hunger_ < 20 && t_ % 600 == 0 && happy_ > 0) --happy_;  // a hungry cat gets grumpy
}

void CatPet::begin(Action a) {
  switch (a) {
    case Action::Play:
      if (energy_ < 15) { say("TOO SLEEPY", 90); return; }
      ballX_ = x_ < W / 2 ? W - 20 : 12;
      ballV_ = x_ < W / 2 ? -2 : 2;
      break;
    case Action::Feed:
      if (hunger_ >= 95) { say("I'M FULL", 90); return; }
      break;
    default: break;
  }
  action_ = a;
  actionT_ = 0;
  bubbleT_ = 0;
}

void CatPet::step(const InputState& in) {
  ++t_;
  decay();
  if (bubbleT_ > 0) --bubbleT_;

  // Menu (disabled while an action plays; A wakes a sleeping cat)
  if (action_ == Action::None) {
    if (in.repeated & BTN_LEFT) menu_ = (menu_ + kActions - 1) % kActions;
    if (in.repeated & BTN_RIGHT) menu_ = (menu_ + 1) % kActions;
    if (in.pressed & BTN_A) begin((Action)(menu_ + 1));
  } else if (action_ == Action::Sleep && (in.pressed & BTN_A)) {
    action_ = Action::None;
    say("YAWN", 60);
  }

  // Action timelines (frames at 60 Hz)
  ++actionT_;
  switch (action_) {
    case Action::Feed:  // cat walks to the bowl, eats, licks lips
      targetX_ = kBowlX + 26;
      if (actionT_ == 150) { hunger_ = clampi(hunger_ + 35, 0, 100); happy_ = clampi(happy_ + 5, 0, 100); }
      if (actionT_ > 200) { action_ = Action::None; say("YUM!", 70); }
      break;
    case Action::Pet:
      if (actionT_ == 60) happy_ = clampi(happy_ + 20, 0, 100);
      if (actionT_ > 170) action_ = Action::None;
      break;
    case Action::Play: {  // yarn rolls, cat chases and pounces
      ballX_ += ballV_;
      if (ballX_ < 8 || ballX_ > W - 18) ballV_ = -ballV_;
      targetX_ = clampi(ballX_ - kCatW / 2, kMinX, kMaxX);
      const int phase = actionT_ % 70;  // pounce every ~1.2 s
      y_ = phase < 20 ? -(phase * (20 - phase)) / 6 : 0;
      if (actionT_ == 240) { happy_ = clampi(happy_ + 30, 0, 100); energy_ = clampi(energy_ - 15, 0, 100); clean_ = clampi(clean_ - 10, 0, 100); }
      if (actionT_ > 260) { action_ = Action::None; y_ = 0; say("AGAIN!", 70); }
      break;
    }
    case Action::Bath:  // bubbles, then a big shake
      if (actionT_ == 200) { clean_ = 100; happy_ = clampi(happy_ - 5, 0, 100); }
      if (actionT_ > 260) { action_ = Action::None; say("HMPH", 60); }
      break;
    case Action::Sleep:  // walks to the cushion, curls up, rests until full of energy
      targetX_ = kCushionX;
      if (x_ != targetX_) { actionT_ = 0; break; }
      if (t_ % 30 == 0 && energy_ < 100) ++energy_;
      if (energy_ >= 100 && actionT_ > 120) { action_ = Action::None; say("GOOD MORNING", 90); }
      break;
    case Action::None: break;
  }

  // Idle life: wander, sit, and ask for what it needs
  if (action_ == Action::None) {
    if (++idleT_ > 180 + (int)(rnd() % 240)) {
      idleT_ = 0;
      targetX_ = kMinX + (int)(rnd() % (uint32_t)(kMaxX - kMinX));
    }
    if (bubbleT_ == 0 && t_ % 420 == 0) {
      if (hunger_ < 30) say("", 150);         // an empty text + need icon = thought bubble
      else if (clean_ < 30) say("", 150);
      else if (energy_ < 25) say("", 150);
      else if (happy_ < 30) say("", 150);
    }
  }
  if (action_ != Action::Bath && action_ != Action::Pet) {
    if (x_ < targetX_) { ++x_; ++walkT_; facing_ = 1; }
    else if (x_ > targetX_) { --x_; ++walkT_; facing_ = -1; }
    else walkT_ = 0;
  }
}


void CatPet::drawRoom(Canvas& c) const {
  const Theme& th = theme();
  const bool asleep = curled();
  const uint16_t wall = asleep ? blend565(th.surface, rgb565(20, 20, 50)) : th.surface;
  const uint16_t floor = th.dark ? rgb565(70, 52, 44) : rgb565(214, 184, 150);
  const uint16_t seam = blend565(floor, th.dark ? rgb565(0, 0, 0) : rgb565(120, 90, 60));
  c.fill(0, 14, W, kFloorY - 14, wall);
  c.fill(0, kFloorY, W, H - 16 - kFloorY, floor);
  c.fill(0, kFloorY, W, 3, th.line);                             // skirting board
  for (int y = kFloorY + 11; y < H - 16; y += 12) c.fill(0, y, W, 1, seam);  // floor boards
  // window: day sky or night with a moon
  const uint16_t sky = asleep ? rgb565(30, 34, 80) : rgb565(150, 205, 255);
  c.fillRound(W - 70, 26, 52, 40, 2, th.line);
  c.fill(W - 67, 29, 46, 34, sky);
  c.fill(W - 45, 29, 2, 34, th.line);
  if (asleep) { c.fillRound(W - 62, 34, 9, 9, 3, rgb565(250, 240, 180)); c.fillRound(W - 59, 33, 8, 8, 3, sky); }
  else c.fillRound(W - 38, 34, 10, 10, 3, rgb565(255, 220, 90));
  // cushion (the bed), lying on the ground line
  const uint16_t cushion = blend565(accent::coral, floor);
  c.fillRound(kCushionX - 6, kGroundY - 9, 20 * kScale + 12, 12, 4, cushion);
  c.fill(kCushionX - 2, kGroundY - 9, 20 * kScale + 4, 2, blend565(cushion, kWhite));
  // bowl standing on the ground line (fish shown while feeding and not yet eaten)
  c.fillRound(kBowlX + 2, kGroundY - 2, 22, 3, 1, seam);          // bowl shadow
  c.fillRound(kBowlX, kGroundY - 9, 26, 8, 3, accent::blue);
  c.fill(kBowlX + 3, kGroundY - 11, 20, 3, blend565(accent::blue, kWhite));
  if (action_ == Action::Feed && actionT_ < 150) {
    const int left = 3 - actionT_ / 50;  // fish pieces disappear as it eats
    for (int i = 0; i < left; ++i) c.fillRound(kBowlX + 4 + i * 6, kGroundY - 14, 5, 4, 1, accent::amber);
  }
}

CatPet::Pose CatPet::pose() const {
  if (action_ == Action::Bath || action_ == Action::Pet) return Pose::Sit;
  if (x_ != targetX_) return Pose::Walk;
  if (action_ == Action::Sleep) return Pose::Curl;
  if (action_ == Action::Play && y_ < 0) return Pose::Walk;  // pounce, stretched out
  return Pose::Sit;
}
int CatPet::poseW() const { return pose() == Pose::Sit ? kCatW : 20 * kScale; }
int CatPet::poseTop() const {
  switch (pose()) {
    case Pose::Walk: return kGroundY - 12 * kScale + y_;
    case Pose::Curl: return kGroundY - 10 * kScale - 2;  // sunk into the cushion a little
    default: return kCatY + y_;
  }
}

static uint16_t catColor(char ch) {
  return ch == 'o' ? kOutline : ch == 'f' ? kFur : ch == 'w' || ch == 'h' ? kWhite : ch == 'p' ? kPink : kEye;
}

void CatPet::drawCat(Canvas& c) const {
  const Pose ps = pose();
  const bool happy = action_ == Action::Pet || (action_ == Action::Feed && actionT_ > 150);
  const bool blink = (t_ % 200) < 8 || ps == Pose::Curl || (action_ == Action::Bath && actionT_ < 200);
  const bool eating = action_ == Action::Feed && actionT_ > 40 && actionT_ < 150 && ps == Pose::Sit;
  const int top = poseTop();
  int x = x_;

  if (ps == Pose::Walk || ps == Pose::Curl) {
    const char* const* rows = ps == Pose::Curl ? kCurl : kWalk[(walkT_ / 8) % 2];
    const int h = ps == Pose::Curl ? 10 : 12;
    const bool flip = ps == Pose::Walk && facing_ < 0;
    const bool breathe = ps == Pose::Curl && (t_ / 45) % 2;  // slow breathing: back rises 1 px
    for (int r = 0; r < h; ++r)
      for (int col = 0; col < 20; ++col) {
        char ch = rows[r][col];
        if (ch == '.') continue;
        if (ch == 'e' && blink) ch = 'f';
        const int sx = flip ? 19 - col : col;
        const int yy = top + r * kScale - (breathe && r < 3 ? 1 : 0);
        c.fill(x + sx * kScale, yy, kScale, kScale, catColor(ch));
      }
    return;
  }

  // sitting, front view
  if (action_ == Action::Bath && actionT_ >= 200) x += (actionT_ / 2) % 2 ? 3 : -3;  // shake
  const int headBob = eating ? ((actionT_ / 10) % 2 ? 5 : 2) : 0;                 // head dips to the bowl
  const int tf = (t_ / 20) % 2;
  for (int r = 0; r < 8; ++r)  // tail behind the body
    for (int col = 0; col < 5; ++col) {
      const char ch = kTail[tf][r][col];
      if (ch == '.') continue;
      c.fill(x + (13 + col) * kScale, top + (7 + r) * kScale, kScale, kScale, ch == 'o' ? kOutline : kFur);
    }
  for (int pass = 0; pass < 2; ++pass)  // body (rows 10+) first, then the head on top of it
    for (int r = pass ? 0 : 10; r < (pass ? 10 : 16); ++r)
      for (int col = 0; col < 16; ++col) {
        char ch = kCat[r][col];
        if (ch == '.') continue;
        // expressions: blink / happy ^^ (eyes become a line)
        if ((ch == 'e' || ch == 'h') && (blink || happy)) ch = (r == (happy ? 5 : 6)) ? 'o' : 'f';
        c.fill(x + col * kScale, top + r * kScale + (pass ? headBob : 0), kScale, kScale, catColor(ch));
      }
  if (happy) { c.fill(x + 3 * kScale, top + 7 * kScale, kScale, kScale, kPink); c.fill(x + 12 * kScale, top + 7 * kScale, kScale, kScale, kPink); }
  if (clean_ < 40) {  // dirt spots when grubby
    c.fill(x + 3 * kScale, top + 12 * kScale, 4, 3, kDirt);
    c.fill(x + 11 * kScale, top + 4 * kScale + headBob, 3, 3, kDirt);
    if (clean_ < 20) c.fill(x + 9 * kScale, top + 13 * kScale, 4, 3, kDirt);
  }
}

void CatPet::draw(Canvas& c) const {
  const Theme& th = theme();
  c.fill(0, 0, W, H, th.bg);
  drawRoom(c);
  if (action_ == Action::Play) {  // yarn ball
    const int by = kGroundY - 10;
    c.fillRound(ballX_ + 1, kGroundY - 2, 9, 3, 1, blend565(theme().dark ? rgb565(70, 52, 44) : rgb565(214, 184, 150), rgb565(0, 0, 0)));
    c.fillRound(ballX_, by, 10, 10, 3, accent::coral);
    c.fill(ballX_ + 2, by + 4, 6, 1, kWhite);
    c.fill(ballX_ + 3, by + 2, 1, 6, kWhite);
    c.fill(ballX_ + (ballV_ > 0 ? -8 : 10), by + 8, 8, 1, accent::coral);  // loose thread
  }
  {  // contact shadow on the ground line; smaller while the cat is in the air
    const uint16_t floor = th.dark ? rgb565(70, 52, 44) : rgb565(214, 184, 150);
    const int sw = poseW() - 6 + y_ / 2;
    c.fillRound(x_ + (poseW() - sw) / 2, kGroundY - 3, sw, 6, 3, blend565(floor, rgb565(0, 0, 0)));
  }
  drawCat(c);

  const int hx = x_ + poseW() / 2;
  const int kCatY = poseTop();  // effects follow the cat's current pose
  if (action_ == Action::Pet) {  // hearts float up
    for (int i = 0; i < 3; ++i) {
      const int k = (actionT_ + i * 25) % 75;
      icon(c, hx - 20 + i * 16, kCatY - 4 - k / 2, 1, accent::coral);
    }
    if ((actionT_ / 30) % 2) c.text(hx + 26, kCatY + 6, "PURR", th.muted);
  }
  if (action_ == Action::Feed && actionT_ > 40 && actionT_ < 150 && (actionT_ / 20) % 2) c.text(hx - 60, kCatY + 4, "NOM", th.muted);
  if (action_ == Action::Bath) {
    if (actionT_ < 200) {  // bubbles rise around the cat
      for (int i = 0; i < 9; ++i) {
        const int bx = x_ + 2 + (int)((i * 37 + actionT_ / 3) % kCatW);
        const int by = kCatY + kCatH - ((actionT_ * (1 + i % 3) + i * 13) % (kCatH + 10));
        c.frame(bx, by, 6, 6, kWater);
        c.dot(bx + 1, by + 1, kWhite);
      }
    } else {  // shake: drops fly out sideways
      for (int i = 0; i < 8; ++i) {
        const int d = (actionT_ - 200) * 2 + i * 3;
        c.fill(hx + (i % 2 ? d : -d), kCatY + 12 + i * 4 - (actionT_ - 200) / 3, 2, 3, kWater);
      }
    }
  }
  if (curled()) {  // floating Zzz
    for (int i = 0; i < 3; ++i) {
      const int k = (actionT_ + i * 40) % 120;
      c.text(clampi(hx + 4 + k / 5, 0, W - 14), kCatY - 4 - k / 3 - i * 2, "Z", accent::teal, 1 + (k > 60));
    }
  }
  // speech / thought bubble
  if (bubbleT_ > 0) {
    int need = -1;
    if (!bubbleText_[0]) need = hunger_ < 30 ? 0 : clean_ < 30 ? 3 : energy_ < 25 ? 2 : 1;
    const int bw = need >= 0 ? 17 : Canvas::textWidth(bubbleText_) + 10;
    const int bx = clampi(hx - bw / 2, 2, W - bw - 2), by = kCatY - 24;
    c.fillRound(bx, by, bw, 15, 3, kWhite);
    c.fill(hx - 2, by + 15, 4, 3, kWhite);
    if (need >= 0) icon(c, bx + 5, by + 4, need, need == 0 ? accent::amber : need == 3 ? accent::blue : need == 2 ? accent::green : accent::coral);
    else c.text(bx + 5, by + 4, bubbleText_, kEye);
  }
  drawHud(c);
  drawMenu(c);
}

void CatPet::drawHud(Canvas& c) const {
  const Theme& th = theme();
  c.fill(0, 0, W, 14, th.surface);
  c.text(6, 3, "NEKO", accent::coral);
  const int vals[4] = {hunger_, happy_, energy_, clean_};
  const int iconOf[4] = {0, 1, 2, 3};
  const uint16_t cols[4] = {accent::amber, accent::coral, accent::green, accent::blue};
  for (int i = 0; i < 4; ++i) {
    const int x = 48 + i * 43;
    icon(c, x, 3, iconOf[i], cols[i]);
    c.fill(x + 10, 5, 28, 4, th.line);
    c.fill(x + 10, 5, vals[i] * 28 / 100, 4, vals[i] < 25 ? accent::warn : cols[i]);
  }
}

void CatPet::drawMenu(Canvas& c) const {
  const Theme& th = theme();
  const int y = H - 16;
  c.fill(0, y, W, 16, th.surface);
  const bool busy = action_ != Action::None;
  for (int i = 0; i < kActions; ++i) {
    const int x = 8 + i * 42;
    const bool sel = i == menu_ && !busy;
    const uint16_t col = busy ? th.line : sel ? kActionColor[i] : th.muted;
    icon(c, x, y + 5, i == 0 ? 0 : i == 1 ? 1 : i == 2 ? 2 : i == 3 ? 3 : 4, col);
    if (sel) c.text(x + 10, y + 5, kActionName[i], col);
  }
  if (action_ == Action::Sleep) c.textRight(W - 6, y + 5, "A WAKE", th.muted);
}
