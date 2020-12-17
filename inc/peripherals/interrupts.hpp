#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include "../main.hpp"
#include "gpio.hpp"
#include <cstdint>

/* TODO:
   Make this more generic so we don't have to have  different functions
   for each timer interrupt etc
*/

namespace Interrupts {
    using InterruptPriority = std::uint8_t;
    
    struct GPIO_INTERRUPT_CONFIG {
        InterruptPriority const priority;
        bool const trigger_on_rise;
        bool const trigger_on_fall;
    };
    
    void enable_gpio_interrupt(GPIO_PIN const& pin, GPIO_INTERRUPT_CONFIG const& config) noexcept;
    void clear_gpio_interrupt_flag(int const pin_number) noexcept;
    void enable_timer6_interrupt (InterruptPriority const priority) noexcept;
    void enable_timer7_interrupt(InterruptPriority const priority) noexcept;
    void enable_usart1_interrupt(InterruptPriority const priority) noexcept;
}

#endif
