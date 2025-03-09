/**
 * @file utils.c
 * @brief Utility functions for the Pico project.
 */

#include "utils.h"

/**
 * @brief Get the current time in milliseconds.
 * @return The current time in milliseconds.
 */
uint32_t millis()
{
    return to_ms_since_boot(get_absolute_time());
}

/**
 * @brief Get the current time in microseconds.
 * @return The current time in microseconds.
 */
uint64_t micros()
{
    return to_us_since_boot(get_absolute_time());
}