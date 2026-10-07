#include <Arduino.h>
#include "runtime.h"

static Runtime g_runtime;

void setup() { g_runtime.begin(); }
void loop() { g_runtime.loop(); }

// Core 1 copies finished emulator frames to the screen while core 0 runs the game.
void setup1() {}
void loop1() { g_runtime.core1(); }
