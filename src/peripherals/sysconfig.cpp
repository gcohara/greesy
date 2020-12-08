#include "../../inc/peripherals/sysconfig.hpp"
#include "../../inc/peripherals/rcc.hpp"

namespace {
    PeripheralStatus static sysconfig_status{ PeripheralStatus::OFF };
    reg32 base{ (reg32) 0x40010000 };
    reg32 sysconfig_config_reg_1{ base };
    reg32 sysconfig_exti_interrupt_register_base{ base + 0x2 };
    
    void enable_sysconfig_clock() noexcept {
        RCC::enable_apb2_clock(RCC::APB2Peripheral::SysConfig);
        sysconfig_status = PeripheralStatus::ON;
    }
}

namespace SysConfig {
    void gpio_interrupt_port_select(GPIO_PIN const& pin) noexcept {
        if (sysconfig_status == PeripheralStatus::OFF){ enable_sysconfig_clock(); }
        auto const address_offset{ pin.pin_number / 4 };
        auto const nibble_to_change{ pin.pin_number % 4 };
        *(sysconfig_exti_interrupt_register_base + address_offset)
            |= (pin.port_number << (4 * nibble_to_change));
    }
}
