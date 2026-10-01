#include "candle.hpp"

#include <algorithm>
#include <cstdlib>
#include <gtest/gtest.h>

TEST(Candle, NoiseIsZeroOnLatticePointsAndBounded)
{
    for (std::uint32_t i = 0; i < 100; ++i) {
        EXPECT_EQ(gran::gradient_noise(i * 256u, 3u), 0);
    }
    for (std::uint32_t x = 0; x < 100'000; ++x) {
        const std::int32_t n = gran::gradient_noise(x, 7u);
        ASSERT_GE(n, -70);
        ASSERT_LE(n, 70);
    }
}

TEST(Candle, LevelStaysInRange)
{
    for (std::uint32_t t = 0; t < 60'000; ++t) {
        const std::uint32_t level = gran::candle_level(t, 0u);
        ASSERT_GE(level, gran::kCandleMinPermille);
        ASSERT_LE(level, gran::kPermilleMax);
    }
}

TEST(Candle, IsDeterministic)
{
    EXPECT_EQ(gran::candle_level(12'345, 2u), gran::candle_level(12'345, 2u));
}

TEST(Candle, ActuallyFlickersAcrossARange)
{
    std::uint32_t lo = gran::kPermilleMax;
    std::uint32_t hi = 0;
    for (std::uint32_t t = 0; t < 60'000; ++t) {
        const std::uint32_t level = gran::candle_level(t, 0u);
        lo = std::min(lo, level);
        hi = std::max(hi, level);
    }
    EXPECT_LT(lo, 300u);
    EXPECT_GT(hi, 850u);
}

TEST(Candle, ChangesSmoothlyFromMsToMs)
{
    for (std::uint32_t t = 1; t < 60'000; ++t) {
        const int step = static_cast<int>(gran::candle_level(t, 1u)) -
                         static_cast<int>(gran::candle_level(t - 1, 1u));
        ASSERT_LE(std::abs(step), 20) << "t=" << t;
    }
}

TEST(Candle, SeedsGiveIndependentFlames)
{
    int differing = 0;
    for (std::uint32_t t = 0; t < 10'000; ++t) {
        if (gran::candle_level(t, 0u) != gran::candle_level(t, 1u)) {
            ++differing;
        }
    }
    EXPECT_GT(differing, 9'000);
}
