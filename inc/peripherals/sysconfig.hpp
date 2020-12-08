#ifndef SYSCONFIG_HPP
#define SYSCONFIG_HPP

#include "../main.hpp"
#include "gpio.hpp"

namespace SysConfig {
    void gpio_interrupt_port_select(GPIO_PIN const& pin) noexcept;
}

#endif
