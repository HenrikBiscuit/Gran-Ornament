#include "app.hpp"

#include "main.h" // CubeMX pin names and HAL handles (e.g. htim1)
#include "tim.h"

#include <array>
#include <cstdint>

extern "C" TIM_HandleTypeDef htim3;

namespace {
void set_duty(std::uint32_t counts)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, counts);
}
} // namespace
extern "C" void app_run(void)
{
    // one-time C++ setup here
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    const std::uint32_t top = __HAL_TIM_GET_AUTORELOAD(&htim3);

    constexpr std::array<std::uint32_t, 4> percent{0, 25, 50, 100};
    for (const auto p : percent) {
        set_duty(top * p / 100);
        HAL_Delay(2000);
    }

    while (true) {
        for (std::uint32_t c = 0; c <= top; c += 4) {
            set_duty(c);
            HAL_Delay(4);
        }
        for (std::uint32_t c = top; c >= 4; c -= 4) {
            set_duty(c);
            HAL_Delay(4);
        }
    }
}