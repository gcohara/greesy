#include "../../inc/peripherals/interrupts.hpp"
#include "../../inc/peripherals/rcc.hpp"
#include "../../inc/peripherals/sysconfig.hpp"

#include <cstddef>



namespace {
    reg32 exti_base{ (reg32) 0x40010400 };
    reg32 interrupt_mask_register_1{ exti_base };
    reg32 exti_rising_trigger_reg_1{ exti_base + 0x2 };
    reg32 exti_falling_trigger_reg_1{ exti_base + 0x3 };
    reg32 exti_pending_register_1{ exti_base + 0x5 };
    reg32 nvic_set_enable_base{ (reg32) 0xe000e100 };
    reg32 nvic_priority_register_base{ (reg32) 0xe000e400 };

    void
    nvic_interrupt_enable
    (int interrupt_number, Interrupts::InterruptPriority priority) noexcept {
        // a way around this could be to create a constexpr function that returns
        if (interrupt_number < 0 || interrupt_number > 84) {
            // error handling
        }
        auto const enable_offset{ interrupt_number / 32 };
        auto const priority_offset{ interrupt_number / 4 };
        *(nvic_set_enable_base + enable_offset) |= (1 << (interrupt_number % 32));
        *(nvic_priority_register_base + priority_offset) |= (priority << (interrupt_number % 4));
    }
    
    // only works for first 32 line numbers
    void
    exti_enable_interrupt_lower_32
    (int line_number, Interrupts::GPIO_INTERRUPT_CONFIG const& config) noexcept {
        if (line_number < 0 || line_number > 31) {
            // error handling goes here
        }
        *interrupt_mask_register_1 |= (1 << line_number);
        if (config.trigger_on_fall) {
            *exti_falling_trigger_reg_1 |= (1 << line_number);
        }
        if (config.trigger_on_rise) {
            *exti_rising_trigger_reg_1 |= (1 << line_number);
        }
    }
}

namespace Interrupts {
    void enable_gpio_interrupt(GPIO_PIN const& pin, GPIO_INTERRUPT_CONFIG const& config) noexcept {
        RCC::enable_apb2_clock(RCC::APB2Peripheral::SysConfig);
        SysConfig::gpio_interrupt_port_select(pin);
        // set whether it's active on rise fall or both
        exti_enable_interrupt_lower_32(pin.pin_number, config);
        // enable in nvic
        if (pin.pin_number >= 0 && pin.pin_number <= 4) {
            nvic_interrupt_enable(pin.pin_number + 6, config.priority);
        }
        else if (pin.pin_number >= 5 && pin.pin_number <= 9){
            nvic_interrupt_enable(23, config.priority);
        }
        else if (pin.pin_number >= 10 && pin.pin_number <= 15){
            nvic_interrupt_enable(40, config.priority);
        }
        else {
            // pin number out of range, something has gone terribly wrong
        }
        
    }

    void enable_timer6_interrupt(InterruptPriority const priority) noexcept {
        nvic_interrupt_enable(54, priority);
    }

    void enable_timer7_interrupt(InterruptPriority const priority) noexcept {
        nvic_interrupt_enable(55, priority);
    }

    void enable_usart1_interrupt(InterruptPriority const priority) noexcept {
        nvic_interrupt_enable(37, priority);
    }
    
    void clear_gpio_interrupt_flag(int pin_number) noexcept {
        *exti_pending_register_1 |= (1 << pin_number);
    }
}
