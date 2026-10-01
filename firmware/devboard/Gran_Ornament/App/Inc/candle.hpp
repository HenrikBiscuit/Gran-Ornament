#pragma once
#include "pwm.hpp"

#include <cstdint>

namespace gran {

// Candle flame from Perlin-style gradient noise, as in
// flameeyes.blog/2020/05/25/fake-candles-and-flame-algorithms. Integer only,
// HAL-free, so it runs on the MCU and is tested on a PC.

// Pseudo-random gradient in [-128, 127] for lattice point `i`. A plain integer
// hash, so the same (seed, i) always gives the same value and no table is needed.
constexpr std::int32_t noise_gradient(std::uint32_t seed, std::uint32_t i)
{
    std::uint32_t h = i * 0x9E37'79B1u + seed * 0x85EB'CA6Bu;
    h ^= h >> 15u;
    h *= 0x2C1B'3C6Du;
    h ^= h >> 12u;
    h *= 0x297A'2D39u;
    h ^= h >> 15u;
    return static_cast<std::int32_t>(h & 0xFFu) - 128;
}

// 1-D gradient noise. `x_q8` is the position in 1/256 lattice units. Smooth,
// 0 on every lattice point, roughly within [-64, 64].
constexpr std::int32_t gradient_noise(std::uint32_t x_q8, std::uint32_t seed)
{
    const std::uint32_t i = x_q8 >> 8u;
    const std::int32_t f = static_cast<std::int32_t>(x_q8 & 0xFFu); // 0..255
    const std::int32_t n0 = noise_gradient(seed, i) * f / 256;
    const std::int32_t n1 = noise_gradient(seed, i + 1u) * (f - 256) / 256;
    // Smoothstep fade 3f^2 - 2f^3, scaled to 0..256, so the slope is 0 at each lattice point.
    const std::int32_t u = f * f * (3 * 256 - 2 * f) / (256 * 256);
    return n0 + u * (n1 - n0) / 256;
}

constexpr std::uint32_t kCandleSlowMs = 1200u; // lattice spacing of the slow sway
constexpr std::uint32_t kCandleFastMs = 300u;  // lattice spacing of the quick flicker
constexpr std::uint32_t kCandleMidPermille = 650u;
constexpr std::uint32_t kCandleMinPermille = 200u; // a flame never goes fully out
constexpr std::int32_t kCandleSlowGain = 5;        // x noise (+-64) = +-320 permille
constexpr std::int32_t kCandleFastGain = 3;        // x noise (+-64) = +-192 permille

// Brightness in permille of one flame at `now_ms`. Different `seed`s give
// independent flames, so the channels don't flicker in lockstep.
constexpr std::uint32_t candle_level(std::uint32_t now_ms, std::uint32_t seed)
{
    const auto t = static_cast<std::uint64_t>(now_ms) * 256u;
    const std::int32_t slow =
        gradient_noise(static_cast<std::uint32_t>(t / kCandleSlowMs), seed * 2u);
    const std::int32_t fast =
        gradient_noise(static_cast<std::uint32_t>(t / kCandleFastMs), seed * 2u + 1u);
    const std::int32_t level = static_cast<std::int32_t>(kCandleMidPermille) +
                               slow * kCandleSlowGain + fast * kCandleFastGain;
    if (level < static_cast<std::int32_t>(kCandleMinPermille)) {
        return kCandleMinPermille;
    }
    if (level > static_cast<std::int32_t>(kPermilleMax)) {
        return kPermilleMax;
    }
    return static_cast<std::uint32_t>(level);
}

} // namespace gran
