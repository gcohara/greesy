#ifndef GPIO_HPP
#define GPIO_HPP

#include "../main.hpp"

reg32 GPIOA_BASE{ (reg32) 0x48000000 };

enum class GPIOPortLetter { A, B, C, D, E, F, };
enum class GPIOInputPUPD { PullUp, PushDown };

class GPIO_PIN;

class GPIO_PORT {
public:
    PeripheralStatus status;
    GPIOPortLetter const port_letter;
    int const letter_number;
    reg32 base;
    reg32 port_mode_register;
    reg32 output_type_register;
    reg32 pullup_pulldown_register;
    reg32 input_data_register;
    reg32 output_data_register;
public:
    GPIO_PORT(GPIOPortLetter const pl);
    void initialise_gp_output(GPIO_PIN const& pin);
    void output_low(GPIO_PIN const& pin);
    void output_high(GPIO_PIN const& pin);
    void toggle_output(GPIO_PIN const& pin);
    void initialise_gp_input(GPIO_PIN const& pin, GPIOInputPUPD const pupd);
    bool read_input_pin(GPIO_PIN const& pin);
    
};

class GPIO_PIN {
public:
    int const pin_number; // would be good to check this is 0<= x < 16
    int const port_number;
private:
    GPIO_PORT & port;
public:
    GPIO_PIN(GPIO_PORT & prt, int const pin_num);
    void initialise_gp_output();
    void initialise_analogue();
    void initialise_gp_input(GPIOInputPUPD const pupd);
    void output_low();
    void output_high();
    void toggle_output();
    bool read();
};

#endif

