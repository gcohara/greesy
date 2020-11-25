#ifndef RCC_HPP
#define RCC_HPP

#include "gpio.hpp"
#include "dac.hpp"

namespace Rcc {   
    enum class APB2Peripheral { SysConfig = 0, };
    enum class APB1Peripheral { DAC1 = 29, DAC2 = 26};

    void enable_apb2_clock(APB2Peripheral peripheral) noexcept;
    void enable_apb1_clock(APB1Peripheral peripheral) noexcept;
    void enable_gpio_port_clock(GPIO_PORT port) noexcept;
}

#endif
