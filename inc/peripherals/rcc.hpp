#ifndef RCC_HPP
#define RCC_HPP

#include "gpio.hpp"
#include "dac.hpp"

namespace RCC {   
    enum class APB2Peripheral { SysConfig = 0, USART1 = 14,};
    enum class APB1Peripheral {
        DAC1 = 29, DAC2 = 26, TIM6 = 4, TIM7 = 5, USART2 = 17, USART3 = 18,
        UART4 = 19, UART5 = 20,
    };
    enum class AHBPeripheral { DMA1 = 0, DMA2 = 1, };

    void enable_apb2_clock(APB2Peripheral peripheral) noexcept;
    void enable_apb1_clock(APB1Peripheral peripheral) noexcept;
    void enable_ahb_clock(AHBPeripheral peripheral) noexcept;
    void enable_gpio_port_clock(GPIO_PORT port) noexcept;
    void set_to_72Mhz() noexcept;
}

#endif
