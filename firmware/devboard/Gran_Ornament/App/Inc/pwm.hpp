#pragma once
#include <cstdint>

namespace gran {

constexpr std::uint32_t kPermilleMax = 1000u;

// Brightness in permille -> timer compare value. HAL-free so it can be
// tested on a PC. `period` is ARR + 1: a compare of `period` keeps the
// output active for the whole cycle, so 1000 permille is truly full on.
// Inputs above kPermilleMax are clamped to full on.
constexpr std::uint32_t duty_to_compare(std::uint32_t period, std::uint32_t permille)
{
    if (permille > kPermilleMax) {
        permille = kPermilleMax;
    }
    // 64-bit intermediate: period * 1000 overflows 32 bits above ~4.29M,
    // which a 32-bit timer could reach.
    return static_cast<std::uint32_t>(static_cast<std::uint64_t>(period) * permille / kPermilleMax);
}

} // namespace gran
