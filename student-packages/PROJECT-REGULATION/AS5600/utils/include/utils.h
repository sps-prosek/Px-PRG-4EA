#ifndef UTILS_H
#define UTILS_H

/* pico SDK libraries */
#include "pico/time.h"

// Macro to convert milliseconds to microseconds
#define MS_TO_US(x) (uint64_t)((x) * 1e3)

// Macro to convert microseconds to milliseconds
#define US_TO_MS(x) (float_t)((x) / 1e3)

// Macro to convert milliseconds to seconds
#define MS_TO_S(x) (float_t)((x) / 1e3)

// Macro to convert seconds to milliseconds
#define S_TO_MS(x) (uint32_t)((x) * 1e3)

// Macro to convert microseconds to seconds
#define US_TO_S(x) (float_t)((x) / 1e6)

// Macro to convert seconds to microseconds
#define S_TO_US(x) (uint64_t)((x) * 1e6)

// Macro to crop a value between a minimum and maximum
#define CROP(x, min, max) (x < min ? min : (x > max ? max : x))

// Function to get the current time in milliseconds
uint32_t millis();

// Function to get the current time in microseconds
uint64_t micros();

#endif // UTILS_H