#pragma once
#include <cstdint>

namespace gran {

// 4-hour auto-off timer. HAL-free: the caller passes the current time
// (e.g. HAL_GetTick() on target), so the logic can be tested on a PC.
class AutoOff {
public:
    static constexpr std::uint32_t kTimeoutMs = 4u * 60u * 60u * 1000u;
    static_assert(kTimeoutMs == 14'400'000u, "4 h in ms");

    constexpr void start(std::uint32_t now_ms) { start_ms_ = now_ms; running_ = true; }
    constexpr void stop() { running_ = false; }
    constexpr bool running() const { return running_; }

    // Unsigned subtraction is wrap-safe: correct even when the 32-bit
    // millisecond tick rolls over (every ~49.7 days).
    constexpr bool expired(std::uint32_t now_ms) const {
        return running_ && (now_ms - start_ms_) >= kTimeoutMs;
    }

private:
    std::uint32_t start_ms_ = 0;
    bool running_ = false;
};

} // namespace gran