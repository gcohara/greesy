#ifndef GPIO_HPP
#define GPIO_HPP

#include "main.hpp"

reg32 GPIOA_BASE{ (reg32) 0x48000000 };

enum class GPIOPortLetter { A, B, C, D, E, F, };

struct GPIO_PIN {
    GPIOPortLetter const port;
    int const pin_number; // would be good to check this is 0<= x < 16
    GPIO_PIN(GPIOPortLetter const pl, int const pin_num):
        port{ pl }, pin_number{ pin_num }{}
};

struct GPIO_PORT {
    PeripheralStatus status;
    GPIOPortLetter const port_letter;
    int const letter_number;
    reg32 base;
    reg32 port_mode_register;
    reg32 output_type_register;
    reg32 output_data_register;
    
    GPIO_PORT(GPIOPortLetter const pl):
    port_letter{ pl },
    letter_number{ static_cast<int>(pl) },
    base{ GPIOA_BASE + (0x100 * letter_number)},
    port_mode_register{ base },
    output_type_register{ base + 0x1 },
    output_data_register{ base + 0x5 } {        
        *RCC_CLOCK_ENABLE_AHB |= (1 << (17 + letter_number));
        status = PeripheralStatus::ON;
    }

    void initialise_gp_output(GPIO_PIN const& pin) {
        *port_mode_register |= (1 << (2 * pin.pin_number));
    }

    void toggle_output(GPIO_PIN const& pin) {
        *output_data_register |= (1 << pin.pin_number);
    }
    
};


#endif
