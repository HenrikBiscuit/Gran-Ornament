#include "fade.hpp"

#include <gtest/gtest.h>

using gran::charge_warning_level;
using gran::Fade;

TEST(Fade, StartsOff)
{
    Fade fade;
    EXPECT_EQ(fade.level(0), 0u);
}

TEST(Fade, FadesInOverOneSecond)
{
    Fade fade;
    fade.fade_in(100);
    EXPECT_EQ(fade.level(100), 0u);
    EXPECT_EQ(fade.level(600), 500u);
    EXPECT_FALSE(fade.done(1099));
    EXPECT_TRUE(fade.done(1100));
    EXPECT_EQ(fade.level(1100), 1000u);
    EXPECT_EQ(fade.level(50'000), 1000u);
}

TEST(Fade, FadesOut)
{
    Fade fade;
    fade.fade_in(0);
    fade.fade_out(2000);
    EXPECT_EQ(fade.level(2000), 1000u);
    EXPECT_EQ(fade.level(2250), 750u);
    EXPECT_EQ(fade.level(3000), 0u);
    EXPECT_TRUE(fade.done(3000));
}

TEST(Fade, FadeOutHalfwayThroughFadeInDoesNotJump)
{
    Fade fade;
    fade.fade_in(0);
    fade.fade_out(400); // at 400 permille
    EXPECT_EQ(fade.level(400), 400u);
    EXPECT_EQ(fade.level(900), 200u);
    EXPECT_EQ(fade.level(1400), 0u);
}

TEST(Fade, WrapSafe)
{
    Fade fade;
    fade.fade_in(0xFFFF'FE00u); // 512 ms before the tick wraps
    EXPECT_EQ(fade.level(0xFFFF'FE00u + 500u), 500u);
    EXPECT_EQ(fade.level(488u), 1000u); // 1000 ms later, after the wrap
}

TEST(ChargeWarning, ThreeSoftPulsesThenOff)
{
    EXPECT_EQ(charge_warning_level(0), 0u);
    EXPECT_EQ(charge_warning_level(300), 500u);
    EXPECT_EQ(charge_warning_level(600), 1000u); // peak of the first pulse
    EXPECT_EQ(charge_warning_level(1200), 0u);   // back to dark between pulses
    EXPECT_EQ(charge_warning_level(1800), 1000u);
    EXPECT_EQ(charge_warning_level(3000), 1000u); // peak of the third pulse
    EXPECT_EQ(charge_warning_level(gran::kChargeWarningMs), 0u);
    EXPECT_EQ(charge_warning_level(10'000), 0u);
}
