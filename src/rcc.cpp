#include "../inc/rcc.hpp"

namespace {
    reg32 rcc_base{ (reg32) 0x40021000 };
    reg32 rcc_clock_control_register{ rcc_base };
    reg32 rcc_clock_config_register{ rcc_base + 0x1 };
    reg32 rcc_clock_config_register2{ rcc_base + 0xB };
    reg32 ahb_clock_enable_register{ rcc_base + 0x5 };
    reg32 apb2_clock_enable_register{ rcc_base + 0x6 };
    reg32 apb1_clock_enable_register{ rcc_base + 0x7 };
    
}

namespace RCC {
    void enable_apb2_clock(APB2Peripheral peripheral) noexcept {
        auto const peripheral_number{ static_cast<int>(peripheral) };
        *apb2_clock_enable_register |= (1 << peripheral_number);
    }

    void enable_apb1_clock(APB1Peripheral peripheral) noexcept {
        auto const peripheral_number{ static_cast<int>(peripheral) };
        *apb1_clock_enable_register |= (1 << peripheral_number);
    }

    void enable_ahb_clock(AHBPeripheral peripheral) noexcept {
        auto const peripheral_number{ static_cast<int>(peripheral)};
        *ahb_clock_enable_register |= (1 << peripheral_number);
    }

    void enable_gpio_port_clock(GPIO_PORT port) noexcept {
        auto const port_number{ static_cast<int>(port.port_letter) };
        *ahb_clock_enable_register |= (1 << (17 + port_number));
    }

    bool hse_ready() {
        return (*rcc_clock_control_register) & (1 << 17);
    }
    
    // this doesn't work dur dur dur
    void set_to_72Mhz() noexcept {
        // set flash latency!!
        (* (reg32) 0x40022000) |= 2;
        // enable HSE clock and set as sysclock
        *rcc_clock_control_register |= (1 << 16);
        while(!hse_ready()) {}
        *rcc_clock_config_register |= 1;
        // hse source to PLL, no prediv
        *rcc_clock_config_register |= (1 << 16);
        *rcc_clock_config_register2 &= (~0 << 4);
        // set PLL to multiply by 9
        *rcc_clock_config_register |= (7 << 18);
        
        // divide apb1 by two
        *rcc_clock_config_register |= (4 << 11);
        
        // pll enable goes last
        *rcc_clock_control_register |= (1 << 24);
        
        *rcc_clock_config_register &= (~0 << 2);
        *rcc_clock_config_register |= 2;
    }
}

