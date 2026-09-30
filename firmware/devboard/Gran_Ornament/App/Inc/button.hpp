#pragma once
#include <cstdint>

namespace gran {

enum class ButtonEvent : std::uint8_t { None, Short, Long };

// Debounced single button with short and long press. HAL-free: the caller
// passes the time and the raw pin state (true = pressed), so the logic can be
// tested on a PC.
//
// A short press is reported on release. A long press is reported while the
// button is still held, as soon as it has been down for kLongPressMs, and the
// release that follows is swallowed.
class Button {
public:
    static constexpr std::uint32_t kDebounceMs = 20u;
    static constexpr std::uint32_t kLongPressMs = 1500u;

    // `held_at_start`: the button is already down at boot, e.g. the press that
    // woke the MCU from Standby. That press is ignored until it is released,
    // so waking up doesn't also change mode or switch straight back off.
    constexpr explicit Button(bool held_at_start = false)
        : raw_(held_at_start), stable_(held_at_start), consumed_(held_at_start)
    {
    }

    constexpr ButtonEvent update(std::uint32_t now_ms, bool raw_pressed)
    {
        if (raw_pressed != raw_) {
            raw_ = raw_pressed;
            raw_since_ms_ = now_ms;
        }

        // Unsigned subtraction is wrap-safe, as in AutoOff.
        if (raw_ != stable_ && (now_ms - raw_since_ms_) >= kDebounceMs) {
            stable_ = raw_;
            if (stable_) {
                pressed_since_ms_ = now_ms;
                consumed_ = false;
                return ButtonEvent::None;
            }
            const bool was_consumed = consumed_;
            consumed_ = false;
            return was_consumed ? ButtonEvent::None : ButtonEvent::Short;
        }

        if (stable_ && !consumed_ && (now_ms - pressed_since_ms_) >= kLongPressMs) {
            consumed_ = true;
            return ButtonEvent::Long;
        }
        return ButtonEvent::None;
    }

    // Debounced state.
    constexpr bool pressed() const { return stable_; }

private:
    bool raw_;
    bool stable_;
    bool consumed_; // this press already produced its event (or is ignored)
    std::uint32_t raw_since_ms_ = 0;
    std::uint32_t pressed_since_ms_ = 0;
};

} // namespace gran
