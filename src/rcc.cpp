#include "../inc/rcc.hpp"

namespace {
    reg32 rcc_base{ (reg32) 0x40021000 };
    reg32 ahb_clock_enable_register{ rcc_base + 0x5 };
    reg32 apb2_clock_enable_register{ rcc_base + 0x6 };
    reg32 apb1_clock_enable_register{ rcc_base + 0x7 };
    
}

namespace Rcc {
    void enable_apb2_clock(APB2Peripheral peripheral) noexcept {
        auto const peripheral_number{ static_cast<int>(peripheral) };
        *apb2_clock_enable_register |= (1 << peripheral_number);
    }

    void enable_apb1_clock(APB1Peripheral peripheral) noexcept {
        auto const peripheral_number{ static_cast<int>(peripheral) };
        *apb1_clock_enable_register |= (1 << peripheral_number);
    }

    void enable_gpio_port_clock(GPIO_PORT port) noexcept {
        auto const port_number{ static_cast<int>(port.port_letter) };
        *ahb_clock_enable_register |= (1 << (17 + port_number));
    }
}

