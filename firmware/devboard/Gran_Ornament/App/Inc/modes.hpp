#pragma once
#include "candle.hpp"
#include "pwm.hpp"

#include <cstdint>

namespace gran {

// Light modes, cycled by a short press. HAL-free, tested on a PC.
//   High, Low: constant brightness.
//   Breathe:   all channels fade up and down together.
//   Candle:    every channel flickers as its own flame (candle.hpp).
enum class Mode : std::uint8_t { High, Low, Breathe, Candle, Count };

constexpr Mode next(Mode m)
{
    const auto n = static_cast<std::uint8_t>(static_cast<std::uint8_t>(m) + 1u);
    return n >= static_cast<std::uint8_t>(Mode::Count) ? Mode::High : static_cast<Mode>(n);
}

constexpr std::uint32_t kHighPermille = kPermilleMax;
constexpr std::uint32_t kLowPermille = 150u;
constexpr std::uint32_t kBreathePeriodMs = 16000u;
constexpr std::uint32_t kBreatheMinPermille = 50u; // never fully dark, so it doesn't look off

// Brightness in permille for `mode` at time `now_ms`. `channel` is the LED
// channel number; only Candle uses it, to give every channel its own flame.
constexpr std::uint32_t mode_level(Mode mode, std::uint32_t now_ms, std::uint32_t channel)
{
    switch (mode) {
    case Mode::Low:
        return kLowPermille;
    case Mode::Breathe: {
        // Triangle wave: up for half the period, down for the other half.
        constexpr std::uint32_t half = kBreathePeriodMs / 2u;
        constexpr std::uint32_t span = kPermilleMax - kBreatheMinPermille;
        const std::uint32_t t = now_ms % kBreathePeriodMs;
        const std::uint32_t ramp = t < half ? t : kBreathePeriodMs - t;
        return kBreatheMinPermille + (ramp * span) / half;
    }
    case Mode::Candle:
        return candle_level(now_ms, channel);
    case Mode::High:
    case Mode::Count:
        break;
    }
    return kHighPermille;
}

// The mode is kept in a TAMP backup register, which survives Standby but not
// a power loss. The magic in the top half tells a stored mode apart from the
// register's reset value (0) or garbage.
constexpr std::uint32_t kModeMagic = 0x4752'0000u; // "GR"

constexpr std::uint32_t encode_mode(Mode m)
{
    return kModeMagic | static_cast<std::uint32_t>(m);
}

constexpr Mode decode_mode(std::uint32_t stored)
{
    const std::uint32_t low = stored & 0xFFFFu;
    if ((stored & 0xFFFF'0000u) != kModeMagic || low >= static_cast<std::uint32_t>(Mode::Count)) {
        return Mode::High;
    }
    return static_cast<Mode>(low);
}

} // namespace gran
