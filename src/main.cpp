#include "../inc/main.hpp"
#include "../inc/gpio.hpp"

auto main() -> int {
    GPIO_PORT portA{ GPIOPortLetter::A };
    GPIO_PORT portE{ GPIOPortLetter::E };
    GPIO_PIN pin7{ GPIOPortLetter::E, 10 };
    portE.initialise_gp_output(pin7);
    portE.toggle_output(pin7);
    while (1);
}
