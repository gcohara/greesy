#include "../inc/main.hpp"
#include "../inc/gpio.hpp"
#include "../inc/sysconfig.hpp"
#include "../inc/interrupts.hpp"
#include "../inc/timers.hpp"
#include "../inc/dac.hpp"
#include "../inc/dma.hpp"
#include <cstddef>
#include <cstdint>
#include <array>

GPIO_PORT static portA{ GPIOPortLetter::A };
GPIO_PORT static portE{ GPIOPortLetter::E };
GPIO_PIN static pin_e_10{ portE, 10 };
GPIO_PIN static pin_a_0{ portA, 0 };

std::size_t constexpr sine_sample_length{ 1024 };

auto main() -> int {
    pin_e_10.initialise_gp_output();
    pin_a_0.initialise_gp_input(GPIOInputPUPD::PushDown);
    Interrupts::enable_gpio_interrupt(pin_a_0, {1, true, false});
    BasicTimers::Config tim6_cfg
        { BasicTimers::MasterModeSelection::Enable,
          false,
          42,
          42,};
    BasicTimers::enable_basic_timer(BasicTimers::TimerNumber::Tim6, tim6_cfg);
    
    int sine_wave[sine_sample_length];
    
    DAC::enable_dac({DAC::DACNumber::DAC1, DAC::ChannelSelection::Tim6, true, true});
    DMA::enable_dac_dma
        (DAC::DACNumber::DAC1, reinterpret_cast<std::uint32_t *>(sine_wave), sine_sample_length);
    while (1) {
        
    }
}

extern "C" {
    void EXTI0_IRQHandler() {
        Interrupts::clear_gpio_interrupt_flag(0);
        pin_e_10.toggle_output();
    }
}
