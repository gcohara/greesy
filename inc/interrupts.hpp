#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include "main.hpp"
#include "gpio.hpp"
#include <cstdint>


namespace Interrupts {
    using InterruptPriority = std::uint8_t;
    using LineNumber = RestrictedInteger<0, 81>;
    
    struct GPIO_INTERRUPT_CONFIG {
        InterruptPriority const priority;
        bool const trigger_on_rise;
        bool const trigger_on_fall;
    };
    
    void enable_gpio_interrupt(GPIO_PIN const& pin, GPIO_INTERRUPT_CONFIG const& config) noexcept;
    void clear_gpio_interrupt_flag(int pin_number) noexcept;
    void enable_timer6_interrupt (InterruptPriority const priority) noexcept;
    void enable_timer7_interrupt(InterruptPriority const priority) noexcept;
}

#endif
