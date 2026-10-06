#pragma once
#include "pwm.hpp"

#include <cstdint>

namespace gran {

// Fade in at switch-on and out at switch-off, so the light never jumps.
// HAL-free: the caller passes the time, so the logic can be tested on a PC.
//
// level() is a multiplier in permille for the mode's brightness. A new fade
// starts from wherever the last one is, so fading out halfway through a
// fade-in doesn't jump either.
class Fade {
public:
    static constexpr std::uint32_t kFadeMs = 1000u;

    constexpr void fade_in(std::uint32_t now_ms) { start(now_ms, kPermilleMax); }
    constexpr void fade_out(std::uint32_t now_ms) { start(now_ms, 0u); }

    constexpr std::uint32_t level(std::uint32_t now_ms) const
    {
        // Unsigned subtraction is wrap-safe, as in AutoOff.
        const std::uint32_t elapsed = now_ms - start_ms_;
        if (elapsed >= kFadeMs) {
            return to_;
        }
        if (to_ >= from_) {
            return from_ + (to_ - from_) * elapsed / kFadeMs;
        }
        return from_ - (from_ - to_) * elapsed / kFadeMs;
    }

    constexpr bool done(std::uint32_t now_ms) const { return (now_ms - start_ms_) >= kFadeMs; }

private:
    constexpr void start(std::uint32_t now_ms, std::uint32_t target)
    {
        from_ = level(now_ms);
        to_ = target;
        start_ms_ = now_ms;
    }

    std::uint32_t start_ms_ = 0;
    std::uint32_t from_ = 0;
    std::uint32_t to_ = 0;
};

// "Time to charge": three soft pulses (fade up, fade down), then off.
constexpr std::uint32_t kChargePulseCount = 3u;
constexpr std::uint32_t kChargePulseMs = 1200u;
constexpr std::uint32_t kChargeWarningMs = kChargePulseCount * kChargePulseMs;

// Brightness in permille, `elapsed_ms` after the warning started.
constexpr std::uint32_t charge_warning_level(std::uint32_t elapsed_ms)
{
    if (elapsed_ms >= kChargeWarningMs) {
        return 0u;
    }
    constexpr std::uint32_t half = kChargePulseMs / 2u;
    const std::uint32_t t = elapsed_ms % kChargePulseMs;
    const std::uint32_t ramp = t < half ? t : kChargePulseMs - t;
    return ramp * kPermilleMax / half;
}

} // namespace gran
