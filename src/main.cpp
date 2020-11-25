#include "../inc/main.hpp"
#include "../inc/gpio.hpp"
#include "../inc/sysconfig.hpp"
#include "../inc/interrupts.hpp"

GPIO_PORT static portA{ GPIOPortLetter::A };
GPIO_PORT static portE{ GPIOPortLetter::E };
GPIO_PIN static pin_e_10{ portE, 10 };
GPIO_PIN static pin_a_0{ portA, 0 };



auto main() -> int {
    pin_e_10.initialise_gp_output();
    pin_a_0.initialise_gp_input(GPIOInputPUPD::PushDown);
    Interrupts::enable_gpio_interrupt(pin_a_0, {1, true, false});
    while (1) {
    }
}

extern "C" {
    void EXTI0_IRQHandler() {
        Interrupts::clear_gpio_interrupt_flag(0);
        pin_e_10.toggle_output();
    }
}
