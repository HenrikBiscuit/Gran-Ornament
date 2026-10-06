#include "battery.hpp"

#include <gtest/gtest.h>

using gran::brightness_scale;
using gran::scale_level;
using gran::vbat_mv_from_raw;

// The uncalibrated voltage in mV, corrected by the board's calibration.
constexpr std::uint32_t calibrated(std::uint32_t mv)
{
    return mv * gran::kVbatCalPermille / 1000u;
}

TEST(VbatMvFromRaw, EndsOfTheRange)
{
    EXPECT_EQ(vbat_mv_from_raw(0), 0u);
    EXPECT_EQ(vbat_mv_from_raw(4095), calibrated(6600u)); // 3.3 V on PA1, doubled
}

TEST(VbatMvFromRaw, LiPoRange)
{
    // 1.5 V and 2.1 V on PA1, rounded down by the integer maths.
    EXPECT_EQ(vbat_mv_from_raw(1861), calibrated(2999u));
    EXPECT_EQ(vbat_mv_from_raw(2606), calibrated(4200u));
}

TEST(VbatMvFromRaw, DevBoardCalibration)
{
    // Measured: the cutoff tripped at 3.73 V while uncalibrated, so a raw
    // reading that used to give ~3.6 V must now give ~3.73 V.
    const std::uint32_t raw = 3600u * gran::kAdcMax / (gran::kDividerGain * gran::kVddMv);
    EXPECT_NEAR(vbat_mv_from_raw(raw), 3725, 5);
}

TEST(BrightnessScale, FullAtOrBelowReference)
{
    EXPECT_EQ(brightness_scale(3000), 1000u);
    EXPECT_EQ(brightness_scale(3600), 1000u);
}

TEST(BrightnessScale, DimsAsBatteryRises)
{
    // Headroom 0.57 V at the reference, 1.17 V at 4.2 V.
    EXPECT_EQ(brightness_scale(4200), 487u);
    EXPECT_EQ(brightness_scale(3700), 850u);
    EXPECT_LT(brightness_scale(4200), brightness_scale(3900));
    EXPECT_LT(brightness_scale(3900), 1000u);
}

TEST(BrightnessScale, NoDivideByZeroForSillyReadings)
{
    EXPECT_EQ(brightness_scale(0), 1000u);
    EXPECT_EQ(brightness_scale(gran::kLedDropMv), 1000u);
}

TEST(ScaleLevel, ScalesAndKeepsOffAsOff)
{
    EXPECT_EQ(scale_level(1000, 487), 487u);
    EXPECT_EQ(scale_level(500, 487), 243u);
    EXPECT_EQ(scale_level(0, 487), 0u);
    EXPECT_EQ(scale_level(1000, 1000), 1000u);
}

using gran::LowBattery;

TEST(LowBattery, StaysOnAboveCutoff)
{
    LowBattery low;
    for (int i = 0; i < 10; ++i) {
        EXPECT_FALSE(low.update(3600));
    }
}

TEST(LowBattery, TripsAfterFiveLowReadingsInARow)
{
    LowBattery low;
    for (int i = 0; i < 4; ++i) {
        EXPECT_FALSE(low.update(3599));
    }
    EXPECT_TRUE(low.update(3599));
    EXPECT_TRUE(low.tripped());
}

TEST(LowBattery, OneGoodReadingResetsTheCount)
{
    LowBattery low;
    for (int i = 0; i < 4; ++i) {
        low.update(3500);
    }
    EXPECT_FALSE(low.update(3700)); // a dip that recovered
    for (int i = 0; i < 4; ++i) {
        EXPECT_FALSE(low.update(3500));
    }
    EXPECT_TRUE(low.update(3500));
}
