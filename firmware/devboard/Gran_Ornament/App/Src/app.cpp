#include "app.hpp"

#include "main.h"
#include "tim.h"

#include <array>
#include <cstdint>

namespace {

struct PwmChannel {
    TIM_HandleTypeDef* tim;
    std::uint32_t channel;
    bool complementary; // CHxN output, needs HAL_TIMEx_PWMN_Start
};

// Front rows, back rows (bottom to top), then the star shared by both sides.
constexpr std::array<PwmChannel, 7> channels{{
    {&htim1,  TIM_CHANNEL_4, false}, // F1 PA11
    {&htim1,  TIM_CHANNEL_3, false}, // F2 PB6
    {&htim1,  TIM_CHANNEL_2, true},  // F3 PB0
    {&htim16, TIM_CHANNEL_1, false}, // B1 PB8
    {&htim3,  TIM_CHANNEL_1, false}, // B2 PA6
    {&htim3,  TIM_CHANNEL_2, false}, // B3 PA7
    {&htim14, TIM_CHANNEL_1, false}, // star PA4
}};

void set_level(const PwmChannel& ch, std::uint32_t permille)
{
    const std::uint32_t period = __HAL_TIM_GET_AUTORELOAD(ch.tim) + 1U;
    __HAL_TIM_SET_COMPARE(ch.tim, ch.channel, period * permille / 1000U);
}

void set_all(std::uint32_t permille)
{
    for (const auto& ch : channels) {
        set_level(ch, permille);
    }
}

void start_pwm()
{
    for (const auto& ch : channels) {
        __HAL_TIM_SET_COMPARE(ch.tim, ch.channel, 0U);
        const HAL_StatusTypeDef status = ch.complementary
            ? HAL_TIMEx_PWMN_Start(ch.tim, ch.channel)
            : HAL_TIM_PWM_Start(ch.tim, ch.channel);
        if (status != HAL_OK) {
            Error_Handler();
        }
    }
}

} // namespace

extern "C" void app_run(void)
{
    start_pwm();

    // Bench test: move the LED string between pins, every channel should
    // show off / dim / medium / full, then a slow fade.
    constexpr std::array<std::uint32_t, 4> steps{0U, 250U, 500U, 1000U};

    while (true) {
        for (const auto level : steps) {
            set_all(level);
            HAL_Delay(2000);
        }
        for (std::uint32_t level = 0U; level <= 1000U; ++level) {
            set_all(level);
            HAL_Delay(3);
        }
        for (std::uint32_t level = 1000U; level > 0U; --level) {
            set_all(level);
            HAL_Delay(3);
        }
    }
}