#pragma once
#include <cstdint>

// Pure logic: no HAL, no registers, so it compiles on a PC too.
// Returns true during the first half of each period.
constexpr bool led_on(std::uint32_t now_ms, std::uint32_t period_ms)
{
    return (now_ms % period_ms) < (period_ms / 2);
}