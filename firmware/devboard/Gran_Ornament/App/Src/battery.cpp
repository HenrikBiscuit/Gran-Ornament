#include "battery.hpp"

#include "adc.h"
#include "main.h"

namespace gran::battery {

void init()
{
    // Trims the ADC's offset. Must run while the ADC is still disabled.
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK) {
        Error_Handler();
    }
}

std::uint32_t read_mv()
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10U);
    return vbat_mv_from_raw(HAL_ADC_GetValue(&hadc1));
}

} // namespace gran::battery
