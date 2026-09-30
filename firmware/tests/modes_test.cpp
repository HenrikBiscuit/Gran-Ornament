#include "modes.hpp"

#include <gtest/gtest.h>

using gran::Mode;

TEST(Modes, NextCyclesAndWraps)
{
    EXPECT_EQ(gran::next(Mode::Steady), Mode::Breathe);
    EXPECT_EQ(gran::next(Mode::Breathe), Mode::Dim);
    EXPECT_EQ(gran::next(Mode::Dim), Mode::Steady);
}

TEST(Modes, SteadyIsFullAndDimIsConstant)
{
    EXPECT_EQ(gran::mode_level(Mode::Steady, 0), gran::kPermilleMax);
    EXPECT_EQ(gran::mode_level(Mode::Dim, 0), gran::kDimPermille);
    EXPECT_EQ(gran::mode_level(Mode::Dim, 123'456), gran::kDimPermille);
}

TEST(Modes, BreatheStaysInRangeAndPeaksMidPeriod)
{
    EXPECT_EQ(gran::mode_level(Mode::Breathe, 0), gran::kBreatheMinPermille);
    EXPECT_EQ(gran::mode_level(Mode::Breathe, gran::kBreathePeriodMs / 2), gran::kPermilleMax);
    EXPECT_EQ(gran::mode_level(Mode::Breathe, gran::kBreathePeriodMs), gran::kBreatheMinPermille);
    for (std::uint32_t t = 0; t < 2 * gran::kBreathePeriodMs; ++t) {
        const std::uint32_t level = gran::mode_level(Mode::Breathe, t);
        ASSERT_GE(level, gran::kBreatheMinPermille);
        ASSERT_LE(level, gran::kPermilleMax);
    }
}

TEST(Modes, EncodeDecodeRoundTrips)
{
    for (const Mode m : {Mode::Steady, Mode::Breathe, Mode::Dim}) {
        EXPECT_EQ(gran::decode_mode(gran::encode_mode(m)), m);
    }
}

TEST(Modes, DecodeRejectsResetValueAndGarbage)
{
    EXPECT_EQ(gran::decode_mode(0u), Mode::Steady); // backup register after power-on
    EXPECT_EQ(gran::decode_mode(0xFFFF'FFFFu), Mode::Steady);
    EXPECT_EQ(gran::decode_mode(gran::kModeMagic | 0x7Fu), Mode::Steady); // out of range
    EXPECT_EQ(gran::decode_mode(0x1234'0001u), Mode::Steady);              // wrong magic
}
