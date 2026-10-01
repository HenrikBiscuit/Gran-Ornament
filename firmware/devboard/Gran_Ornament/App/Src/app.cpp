#include "app.hpp"

#include "auto_off.hpp"
#include "button.hpp"
#include "main.h"
#include "modes.hpp"
#include "power.hpp"
#include "pwm.hpp"
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
    {&htim1, TIM_CHANNEL_4, false},  // F1 PA11
    {&htim1, TIM_CHANNEL_3, false},  // F2 PB6
    {&htim16, TIM_CHANNEL_1, false}, // F3 PB8
    {&htim14, TIM_CHANNEL_1, false}, // B1 PA4
    {&htim3, TIM_CHANNEL_1, false},  // B2 PA6
    {&htim3, TIM_CHANNEL_2, false},  // B3 PA7
    {&htim1, TIM_CHANNEL_2, true},   // star PB0 (CH2N)
}};

void set_level(const PwmChannel& ch, std::uint32_t permille)
{
    const std::uint32_t period = __HAL_TIM_GET_AUTORELOAD(ch.tim) + 1U;
    __HAL_TIM_SET_COMPARE(ch.tim, ch.channel, gran::duty_to_compare(period, permille));
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
        const HAL_StatusTypeDef status = ch.complementary ? HAL_TIMEx_PWMN_Start(ch.tim, ch.channel)
                                                          : HAL_TIM_PWM_Start(ch.tim, ch.channel);
        if (status != HAL_OK) {
            Error_Handler();
        }
    }
}

bool button_down()
{
    return HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_RESET; // to GND, pull-up
}

[[noreturn]] void switch_off(gran::Button& button)
{
    set_all(0U);
    // Standby wakes on the falling edge, so let go of the button first.
    while (button.pressed()) {
        button.update(HAL_GetTick(), button_down());
    }
    gran::power::enter_standby();
}

} // namespace

extern "C" void app_run(void)
{
    gran::power::init();
    start_pwm();

    gran::Mode mode = gran::decode_mode(gran::power::read_backup());
    gran::Button button(button_down());
    gran::AutoOff auto_off;
    auto_off.start(HAL_GetTick());

    while (true) {
        const std::uint32_t now = HAL_GetTick();

        switch (button.update(now, button_down())) {
        case gran::ButtonEvent::Short:
            mode = gran::next(mode);
            gran::power::write_backup(gran::encode_mode(mode));
            auto_off.start(now); // any press restarts the 4 h timer
            break;
        case gran::ButtonEvent::Long:
            switch_off(button);
        case gran::ButtonEvent::None:
            break;
        }

        if (auto_off.expired(now)) {
            switch_off(button);
        }

        std::uint32_t index = 0U;
        for (const auto& ch : channels) {
            set_level(ch, gran::mode_level(mode, now, index));
            ++index;
        }
        HAL_Delay(1);
    }
}
