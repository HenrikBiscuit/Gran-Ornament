#include "button.hpp"

#include <gtest/gtest.h>

using gran::Button;
using gran::ButtonEvent;

namespace {

// Holds the button at `pressed` from `from` to `to` (inclusive), 1 ms steps,
// and returns the first event seen (or None).
ButtonEvent hold(Button& b, bool pressed, std::uint32_t from, std::uint32_t to)
{
    ButtonEvent first = ButtonEvent::None;
    // Stops at `to` rather than testing t <= to, which never ends for
    // to == 0xFFFF'FFFF.
    for (std::uint32_t t = from;; ++t) {
        const ButtonEvent e = b.update(t, pressed);
        if (first == ButtonEvent::None) {
            first = e;
        }
        if (t == to) {
            return first;
        }
    }
}

} // namespace

TEST(Button, ShortPressReportedOnRelease)
{
    Button b;
    EXPECT_EQ(hold(b, true, 0, 200), ButtonEvent::None);
    EXPECT_TRUE(b.pressed());
    EXPECT_EQ(hold(b, false, 201, 300), ButtonEvent::Short);
    EXPECT_FALSE(b.pressed());
}

TEST(Button, BounceShorterThanDebounceIsIgnored)
{
    Button b;
    EXPECT_EQ(hold(b, true, 0, Button::kDebounceMs - 2), ButtonEvent::None);
    EXPECT_EQ(hold(b, false, Button::kDebounceMs - 1, 200), ButtonEvent::None);
    EXPECT_FALSE(b.pressed());
}

TEST(Button, ChatterDuringPressGivesOneShort)
{
    Button b;
    int shorts = 0;
    for (std::uint32_t t = 0; t < 10; ++t) { // contact bounce
        shorts += b.update(t, (t % 2) == 0) == ButtonEvent::Short ? 1 : 0;
    }
    for (std::uint32_t t = 10; t < 200; ++t) {
        shorts += b.update(t, true) == ButtonEvent::Short ? 1 : 0;
    }
    for (std::uint32_t t = 200; t < 210; ++t) { // release bounce
        shorts += b.update(t, (t % 2) == 1) == ButtonEvent::Short ? 1 : 0;
    }
    for (std::uint32_t t = 210; t < 400; ++t) {
        shorts += b.update(t, false) == ButtonEvent::Short ? 1 : 0;
    }
    EXPECT_EQ(shorts, 1);
}

TEST(Button, LongPressFiresWhileHeldAndSwallowsRelease)
{
    Button b;
    // Pressed state is confirmed at kDebounceMs, long fires kLongPressMs later.
    const std::uint32_t long_at = Button::kDebounceMs + Button::kLongPressMs;
    EXPECT_EQ(hold(b, true, 0, long_at - 1), ButtonEvent::None);
    EXPECT_EQ(b.update(long_at, true), ButtonEvent::Long);
    EXPECT_EQ(hold(b, true, long_at + 1, long_at + 3000), ButtonEvent::None);
    EXPECT_EQ(hold(b, false, long_at + 3001, long_at + 3100), ButtonEvent::None);
    EXPECT_FALSE(b.pressed());
}

TEST(Button, WakePressIsIgnoredUntilReleased)
{
    Button b(true); // held at boot
    EXPECT_TRUE(b.pressed());
    EXPECT_EQ(hold(b, true, 0, 5000), ButtonEvent::None);     // no long
    EXPECT_EQ(hold(b, false, 5001, 5100), ButtonEvent::None); // no short
    // The next press works normally.
    EXPECT_EQ(hold(b, true, 5101, 5200), ButtonEvent::None);
    EXPECT_EQ(hold(b, false, 5201, 5300), ButtonEvent::Short);
}

TEST(Button, SurvivesTickWraparound)
{
    Button b;
    const std::uint32_t start = 0xFFFF'FFF0u;
    EXPECT_EQ(hold(b, true, start, 0xFFFF'FFFFu), ButtonEvent::None);
    EXPECT_EQ(hold(b, true, 0, 100), ButtonEvent::None);
    EXPECT_EQ(hold(b, false, 101, 200), ButtonEvent::Short);
}
