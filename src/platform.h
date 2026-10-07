#pragma once
#include <stdint.h>
#include <stddef.h>

// Time helpers.
#include <Arduino.h>
#include <pico/time.h>
inline uint64_t plat_now_us() { return time_us_64(); }
inline void plat_sleep_us(uint32_t us) { sleep_us(us); }  // WFE-based, low power
