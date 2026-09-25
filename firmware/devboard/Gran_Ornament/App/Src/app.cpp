#include "app.hpp"
#include "main.h"   // CubeMX pin names and HAL handles (e.g. htim1)
#include "output_pin.hpp"
#include "blink.hpp"


namespace 
{
    constexpr std::uint32_t kBlinkPeriodMs = 1000;   // 500 ms on, 500 ms off
}

extern "C" void app_run(void) 
{
    // one-time C++ setup here

    const OutputPin testLed{LED_Test_GPIO_Port, LED_Test_Pin};

    while (true) 
    {
        testLed.set(led_on(HAL_GetTick(), kBlinkPeriodMs));
    }
}