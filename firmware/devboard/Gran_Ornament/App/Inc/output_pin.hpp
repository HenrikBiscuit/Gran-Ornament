#pragma once

#include "main.h"   // HAL types, plus CubeMX pin names
class OutputPin 
{
public:
    OutputPin(GPIO_TypeDef* port, uint16_t pin) : port_(port), pin_(pin) {}

    void on() const     { HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET); }
    void off() const    { HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET); }
    void toggle() const { HAL_GPIO_TogglePin(port_, pin_); }
    void set(bool state) const { state ? on() : off(); }

private:
    GPIO_TypeDef* const port_;   // const: a pin object never changes which pin it is
    const uint16_t pin_;
};