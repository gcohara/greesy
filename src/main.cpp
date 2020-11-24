#include "../inc/main.hpp"
#include "../inc/gpio.hpp"

auto main() -> int {
    GPIO_PORT portA{ GPIOPortLetter::A };
    GPIO_PORT portE{ GPIOPortLetter::E };
    GPIO_PIN pin_e_10{ portE, 10 };
    GPIO_PIN pin_a_0{ portA, 0 };

    pin_e_10.initialise_gp_output();
    pin_a_0.initialise_gp_input(GPIOInputPUPD::PushDown);
    
    while (1) {
        if (pin_a_0.read()){ pin_e_10.output_high(); }
        else { pin_e_10.output_low(); }
    }
}
