#include "pwm.hpp"

#include <gtest/gtest.h>

using gran::duty_to_compare;

TEST(DutyToCompare, ZeroIsOff)
{
    EXPECT_EQ(duty_to_compare(1000, 0), 0u);
}

TEST(DutyToCompare, FullIsWholePeriod)
{
    // Compare must equal ARR + 1 (not ARR) for 100 % duty.
    EXPECT_EQ(duty_to_compare(1000, 1000), 1000u);
    EXPECT_EQ(duty_to_compare(65536, 1000), 65536u);
}

TEST(DutyToCompare, ScalesLinearly)
{
    EXPECT_EQ(duty_to_compare(1000, 250), 250u);
    EXPECT_EQ(duty_to_compare(2000, 500), 1000u);
}

TEST(DutyToCompare, RoundsDown)
{
    EXPECT_EQ(duty_to_compare(999, 500), 499u);
}

TEST(DutyToCompare, ClampsAboveFull)
{
    EXPECT_EQ(duty_to_compare(1000, 1200), 1000u);
    EXPECT_EQ(duty_to_compare(1000, UINT32_MAX), 1000u);
}

TEST(DutyToCompare, NoOverflowOnLargePeriod)
{
    // 32-bit timer at max: period * 1000 would overflow a uint32_t.
    constexpr std::uint32_t period = 0xFFFF'FFFFu;
    EXPECT_EQ(duty_to_compare(period, 1000), period);
    EXPECT_EQ(duty_to_compare(period, 500), period / 2u);
}

static_assert(duty_to_compare(1000, 1000) == 1000u, "usable at compile time");
