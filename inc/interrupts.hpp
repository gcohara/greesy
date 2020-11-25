#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include "main.hpp"
#include "gpio.hpp"

struct INTERRUPT_CONFIG {
    int const priority;
    bool trigger_on_rise;
    bool trigger_on_fall;
};

namespace Interrupts {
    void enable_gpio_interrupt(GPIO_PIN const& pin, INTERRUPT_CONFIG const& config) noexcept;
    void clear_gpio_interrupt_flag(int pin_number) noexcept;
};

#endif
