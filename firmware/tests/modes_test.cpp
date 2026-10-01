#include "modes.hpp"

#include <gtest/gtest.h>

using gran::Mode;

TEST(Modes, NextCyclesAndWraps)
{
    EXPECT_EQ(gran::next(Mode::High), Mode::Low);
    EXPECT_EQ(gran::next(Mode::Low), Mode::Breathe);
    EXPECT_EQ(gran::next(Mode::Breathe), Mode::Candle);
    EXPECT_EQ(gran::next(Mode::Candle), Mode::High);
}

TEST(Modes, HighAndLowAreConstant)
{
    for (const std::uint32_t t : {0u, 123'456u}) {
        EXPECT_EQ(gran::mode_level(Mode::High, t, 0), gran::kHighPermille);
        EXPECT_EQ(gran::mode_level(Mode::Low, t, 0), gran::kLowPermille);
    }
    EXPECT_LT(gran::kLowPermille, gran::kHighPermille);
}

TEST(Modes, BreatheStaysInRangeAndPeaksMidPeriod)
{
    EXPECT_EQ(gran::mode_level(Mode::Breathe, 0, 0), gran::kBreatheMinPermille);
    EXPECT_EQ(gran::mode_level(Mode::Breathe, gran::kBreathePeriodMs / 2, 0), gran::kPermilleMax);
    EXPECT_EQ(gran::mode_level(Mode::Breathe, gran::kBreathePeriodMs, 0),
              gran::kBreatheMinPermille);
    for (std::uint32_t t = 0; t < 2 * gran::kBreathePeriodMs; ++t) {
        const std::uint32_t level = gran::mode_level(Mode::Breathe, t, 0);
        ASSERT_GE(level, gran::kBreatheMinPermille);
        ASSERT_LE(level, gran::kPermilleMax);
    }
}

TEST(Modes, BreatheIsTheSameOnEveryChannel)
{
    EXPECT_EQ(gran::mode_level(Mode::Breathe, 1'234, 0), gran::mode_level(Mode::Breathe, 1'234, 6));
}

TEST(Modes, CandleUsesOneFlamePerChannel)
{
    EXPECT_EQ(gran::mode_level(Mode::Candle, 1'234, 3), gran::candle_level(1'234, 3));
    EXPECT_NE(gran::mode_level(Mode::Candle, 1'234, 0), gran::mode_level(Mode::Candle, 1'234, 6));
}

TEST(Modes, EncodeDecodeRoundTrips)
{
    for (const Mode m : {Mode::High, Mode::Low, Mode::Breathe, Mode::Candle}) {
        EXPECT_EQ(gran::decode_mode(gran::encode_mode(m)), m);
    }
}

TEST(Modes, DecodeRejectsResetValueAndGarbage)
{
    EXPECT_EQ(gran::decode_mode(0u), Mode::High); // backup register after power-on
    EXPECT_EQ(gran::decode_mode(0xFFFF'FFFFu), Mode::High);
    EXPECT_EQ(gran::decode_mode(gran::kModeMagic | 0x7Fu), Mode::High); // out of range
    EXPECT_EQ(gran::decode_mode(0x1234'0001u), Mode::High);             // wrong magic
}
