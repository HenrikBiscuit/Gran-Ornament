#pragma once
#include "pwm.hpp"

#include <cstdint>

namespace gran {

// Battery voltage and LED brightness compensation. The constexpr part is
// HAL-free and tested on a PC; gran::battery::init/read_mv are the ADC glue.

constexpr std::uint32_t kVddMv = 3300u;    // ADC reference (LDO output)
constexpr std::uint32_t kAdcMax = 4095u;   // 12 bit
constexpr std::uint32_t kDividerGain = 2u; // 1M/1M divider halves VBAT

// Correction in permille for this board's divider and VDD. Dev board,
// measured 2026-10-06: in Low mode (almost no lead drop) the 3.6 V cutoff
// tripped at 3.73 V on the bench supply, 3.5 % low. ~0.8 % of that is VDD
// (3.326 V), the rest is the hand-wired divider. Measure the PCB separately.
constexpr std::uint32_t kVbatCalPermille = 1035u;

// Raw ADC reading of VBAT_SENSE -> battery voltage in mV.
constexpr std::uint32_t vbat_mv_from_raw(std::uint32_t raw)
{
    return raw * kDividerGain * kVddMv / kAdcMax * kVbatCalPermille / 1000u;
}

// The LED current is set by the voltage left across the series resistor:
// VBAT minus the LED's forward voltage and the pin's low voltage (measured at
// 2.70 V + 0.33 V, see docs/bring-up). A fuller battery pushes more current,
// so the duty is scaled down to look as bright as at kVbatRefMv.
constexpr std::uint32_t kLedDropMv = 2700u + 330u;
constexpr std::uint32_t kVbatRefMv = 3600u; // brightness stays constant down to here

// Brightness scale in permille for the battery voltage `vbat_mv`. 1000 at or
// below kVbatRefMv (nothing left to compensate with), ~487 at 4.2 V.
constexpr std::uint32_t brightness_scale(std::uint32_t vbat_mv)
{
    if (vbat_mv <= kVbatRefMv) {
        return kPermilleMax;
    }
    return (kVbatRefMv - kLedDropMv) * kPermilleMax / (vbat_mv - kLedDropMv);
}

// Applies a scale from brightness_scale() to a level in permille.
constexpr std::uint32_t scale_level(std::uint32_t permille, std::uint32_t scale)
{
    return permille * scale / kPermilleMax;
}

// Low-battery cutoff: below kCutoffMv the ornament switches off, leaving
// charge for storage. It trips only after kReadingsToTrip low readings in a
// row, so a single dip under LED load doesn't switch it off.
class LowBattery {
public:
    static constexpr std::uint32_t kCutoffMv = 3600u;
    static constexpr std::uint32_t kReadingsToTrip = 5u;

    // Feed every battery reading. Returns true once the cutoff has tripped.
    constexpr bool update(std::uint32_t vbat_mv)
    {
        if (vbat_mv >= kCutoffMv) {
            low_count_ = 0u;
        } else if (low_count_ < kReadingsToTrip) {
            ++low_count_;
        }
        return tripped();
    }

    constexpr bool tripped() const { return low_count_ >= kReadingsToTrip; }

private:
    std::uint32_t low_count_ = 0u;
};

namespace battery {

// Call once at boot, after the CubeMX init. Calibrates the ADC.
void init();

// Reads VBAT_SENSE once and returns the battery voltage in mV.
std::uint32_t read_mv();

} // namespace battery

} // namespace gran
