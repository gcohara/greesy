#ifndef RCC_HPP
#define RCC_HPP

#include "gpio.hpp"

namespace Rcc {   
    enum class APB2Peripheral { SysConfig, };

    void enable_apb2_clock(APB2Peripheral peripheral) noexcept;
    void enable_gpio_port_clock(GPIO_PORT port) noexcept;
}

#endif
