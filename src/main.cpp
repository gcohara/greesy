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
GPIO_PIN static pin_a_4{ portA, 4 };
std::size_t constexpr clock_speed{ 8'000'000 };
std::size_t constexpr desired_sample_rate{ 44'100 };
std::size_t constexpr autoreload_value{ clock_speed / desired_sample_rate };


std::size_t constexpr sine_sample_length{ 100 };
int static sine_wave[sine_sample_length] {
    307,326,345,364,383,402,420,437,455,471,487,502,517,531,543,555,566,576,585,592,
        599,604,608,611,613,614,613,611,608,604,599,592,585,576,566,555,543,531,517,502,
        487,471,455,437,420,402,383,364,345,326,307,287,268,249,230,212,194,176,159,142,
        126,111,96,83,70,58,47,37,29,21,15,9,5,2,0,0,0,2,5,9,
        15,21,29,37,47,58,70,83,96,111,126,142,159,176,194,212,230,249,268,287,
        };

auto main() -> int {
    pin_e_10.initialise_gp_output();
    pin_a_0.initialise_gp_input(GPIOInputPUPD::PushDown);
    pin_a_4.initialise_analogue();
    
    Interrupts::enable_gpio_interrupt(pin_a_0, {1, true, false});
    // SysConfig::tim6_dac1_remaP();
    
    BasicTimers::Config constexpr tim6_cfg{
        BasicTimers::MasterModeSelection::Update,
        false,
        0,
        autoreload_value,
    };
    BasicTimers::enable_basic_timer(BasicTimers::TimerNumber::Tim6, tim6_cfg);
    DAC::enable_dac({DAC::DACNumber::DAC1, DAC::ChannelSelection::Tim6, true, true});
    DMA::enable_dac_dma(
        DAC::DACNumber::DAC1,
        reinterpret_cast<std::uint32_t *>(sine_wave),
        sine_sample_length
        );
    while (1) {
        
    }
}


extern "C" {
    void EXTI0_IRQHandler() {
        Interrupts::clear_gpio_interrupt_flag(0);
        pin_e_10.toggle_output();
    }
}
