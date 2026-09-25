#include <gtest/gtest.h>
#include "auto_off.hpp"

using gran::AutoOff;

TEST(AutoOff, NotExpiredBeforeStart) {
    AutoOff t;
    EXPECT_FALSE(t.running());
    EXPECT_FALSE(t.expired(AutoOff::kTimeoutMs * 2));
}

TEST(AutoOff, ExpiresExactlyAtFourHours) {
    AutoOff t;
    t.start(1000);
    EXPECT_FALSE(t.expired(1000 + AutoOff::kTimeoutMs - 1));
    EXPECT_TRUE(t.expired(1000 + AutoOff::kTimeoutMs));
}

TEST(AutoOff, SurvivesTickWraparound) {
    AutoOff t;
    const std::uint32_t start = 0xFFFF'FFF0u;          // 16 ms before rollover
    t.start(start);
    EXPECT_FALSE(t.expired(0x10u));                    // 32 ms later, after wrap
    EXPECT_TRUE(t.expired(static_cast<std::uint32_t>(start + AutoOff::kTimeoutMs)));
}

TEST(AutoOff, StopCancels) {
    AutoOff t;
    t.start(0);
    t.stop();
    EXPECT_FALSE(t.expired(AutoOff::kTimeoutMs));
}