#include "../inc/main.hpp"
#include "../inc/gpio.hpp"
#include "../inc/sysconfig.hpp"
#include "../inc/interrupts.hpp"

GPIO_PORT static portA{ GPIOPortLetter::A };
GPIO_PORT static portE{ GPIOPortLetter::E };
GPIO_PIN static pin_e_10{ portE, 10 };
GPIO_PIN static pin_a_0{ portA, 0 };

extern void (*_spreinit_array []) (void) __attribute__((weak));
extern void (*_epreinit_array [])(void) __attribute__((weak));
extern void (*_sinit_array [])(void) __attribute__((weak));
extern void (*_einit_array [])(void) __attribute__((weak));

auto main() -> int {
    int cpp_count = 0;
    int cpp_size = &(_epreinit_array[0]) - &(_spreinit_array[0]);
    for (cpp_count = 0; cpp_count < cpp_size; ++cpp_count) {
        _spreinit_array[cpp_count]();
    }
    // ('init_array' sections call static constructors)
    cpp_size = &(_einit_array[0]) - &(_sinit_array[0]);
    for (cpp_count = 0; cpp_count < cpp_size; ++cpp_count) {
        _sinit_array[cpp_count]();
    }

    pin_e_10.initialise_gp_output();
    pin_a_0.initialise_gp_input(GPIOInputPUPD::PushDown);
    Interrupts::enable_gpio_interrupt(pin_a_0, {1, true, false});
    while (1) {
        // if (pin_a_0.read()) { pin_e_10.output_high(); }
        // else { pin_e_10.output_low(); }
    }
}

extern "C" {
    void EXTI0_IRQHandler() {
        Interrupts::clear_gpio_interrupt_flag(0);
        pin_e_10.toggle_output();
    }
}
