#include "../../inc/peripherals/gpio.hpp"
#include "../../inc/peripherals/rcc.hpp"
#include <cstdint>

GPIO_PORT::GPIO_PORT(GPIOPortLetter const pl):
    port_letter{ pl },
    letter_number{ static_cast<int>(pl) },
    base{ GPIOA_BASE + (0x100 * letter_number)},
    port_mode_register{ base },
    output_type_register{ base + 0x1 },
    pullup_pulldown_register{ base + 0x3 },
    input_data_register{ base + 0x4 },
    output_data_register{ base + 0x5 },
    alternate_function_low{ base + 0x8 },
    alternate_function_high{ base + 0x9 }
{
    RCC::enable_gpio_port_clock(*this);
    status = PeripheralStatus::ON;
}

void GPIO_PORT::initialise_gp_output(GPIO_PIN const& pin) {
    *port_mode_register &= ~(3 << (pin.pin_number * 2));
    *port_mode_register |= (1 << (pin.pin_number * 2));
}

void GPIO_PORT::output_high(GPIO_PIN const& pin) {
    *output_data_register |= (1 << pin.pin_number);
}

void GPIO_PORT::output_low(GPIO_PIN const& pin) {
    *output_data_register &= ~(1 << pin.pin_number);
}

void GPIO_PORT::toggle_output(GPIO_PIN const& pin) {
    *output_data_register ^= (1 << pin.pin_number);
}
    
void GPIO_PORT::initialise_gp_input(GPIO_PIN const& pin, GPIOInputPUPD const pupd) {
    if (pupd == GPIOInputPUPD::PushDown) {
        *pullup_pulldown_register |= (2 << (2 * pin.pin_number));
    } else {
        *pullup_pulldown_register |= (1 << (2 * pin.pin_number));
    }
}

bool GPIO_PORT::read_input_pin(GPIO_PIN const& pin) {
    return (*input_data_register & (1 << pin.pin_number));
}

GPIO_PIN::GPIO_PIN(GPIO_PORT & prt, int const pin_num):
    pin_number{ pin_num }, port_number{ static_cast<int>(prt.port_letter)}, port{ prt } {}

void GPIO_PIN::initialise_gp_output() {
    port.initialise_gp_output(*this);
}

void GPIO_PIN::initialise_gp_input(GPIOInputPUPD const pupd) {
    port.initialise_gp_input(*this, pupd);
}

void GPIO_PIN::output_low() {
    port.output_low(*this);
}

void GPIO_PIN::output_high() {
    port.output_high(*this);
}

void GPIO_PIN::toggle_output() {
    port.toggle_output(*this);
}

bool GPIO_PIN::read() {
    return port.read_input_pin(*this);
}

void GPIO_PIN::initialise_analogue() {
    *port.port_mode_register |= (3 << (2 * pin_number));
}

void GPIO_PIN::set_alternate_function(std::uint8_t const af_num) noexcept {
    auto const pin_num{ this->pin_number };
    auto const port{ this->port };
    // Clear mode, then set to alternate function mode
    *port.port_mode_register &= ~(3 << (pin_num * 2));
    *port.port_mode_register |= (2 << (pin_num * 2));
    if (pin_num >= 8) {
        *port.alternate_function_high |= ((af_num % 16) << ((pin_num % 8) * 4));
    }
    else {
        *port.alternate_function_low |= ((af_num % 16) << ((pin_num % 8) * 4));
    }
}

void GPIO_PIN::set_pullup_pulldown(GPIOInputPUPD const pupd) noexcept {
    auto const pin_num{ this-> pin_number };
    auto const port{ this->port };
    *port.pullup_pulldown_register &= ~(3 << (pin_num * 2));
    *port.pullup_pulldown_register |= (static_cast<std::uint8_t>(pupd) << 2);
}
