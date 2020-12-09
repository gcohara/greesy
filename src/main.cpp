#include "../inc/main.hpp"
#include "../inc/peripherals/gpio.hpp"
#include "../inc/peripherals/sysconfig.hpp"
#include "../inc/peripherals/interrupts.hpp"
#include "../inc/peripherals/timers.hpp"
#include "../inc/peripherals/dac.hpp"
#include "../inc/peripherals/rcc.hpp"
#include "../inc/tables.hpp"
#include "../inc/synth.hpp"
#include <cstddef>
#include <cstdint>
#include <array>
#include <cmath>


GPIO_PORT static portA{ GPIOPortLetter::A };
GPIO_PORT static portE{ GPIOPortLetter::E };
GPIO_PIN static led{ portE, 10 };
GPIO_PIN static push_button{ portA, 0 };
GPIO_PIN static audio_out{ portA, 4 };
BasicTimers::BasicTimer<BasicTimers::TimerNumber::Tim6> static timer_6;
BasicTimers::BasicTimer<BasicTimers::TimerNumber::Tim7> static envelope_timer;

// global constants
std::size_t constexpr clock_speed{ 72'000'000 };
std::size_t constexpr desired_sample_rate{ 44'100 };
std::size_t constexpr autoreload_value{ clock_speed / desired_sample_rate };
std::float_t constexpr extern base_frequency {
                           static_cast<float>(desired_sample_rate) / sine_sample_length };



auto main() -> int {
    RCC::set_to_72Mhz();        // gotta go fast
    // Define our configs for the peripherals we will be using
    DAC::Config constexpr dac1_cfg{
        .chan_num{ DAC::ChannelSelect::Channel1 },
        .buffer_disable{ true },
        .trigger_enable{ false },
    };
    BasicTimers::Config constexpr tim6_cfg{
        .psc_val{ 0 },
        .autorel_val{ autoreload_value },
        .interrupt{ true }
    };
    BasicTimers::Config constexpr envelope_timer_config{
        .psc_val{ 500 },
        .autorel_val{ autoreload_value },
        .interrupt{ true }
    };
    // GPIO config, use the pushbutton as a trigger
    led.initialise_gp_output();
    push_button.initialise_gp_input(GPIOInputPUPD::PushDown);
    Interrupts::enable_gpio_interrupt(push_button, {1, true, false});
    audio_out.initialise_analogue();
    // Enable peripherals and other interrupts
    DAC::enable_dac(dac1_cfg);
    timer_6.enable(tim6_cfg);
    Interrupts::enable_timer6_interrupt(1);
    envelope_timer.enable(envelope_timer_config);
    Interrupts::enable_timer7_interrupt(0);
    
    while (1);
}






extern "C" {
    void EXTI0_IRQHandler() noexcept {
        Interrupts::clear_gpio_interrupt_flag(0);
        // freq += 100;
        // play_frequency(freq);
        Synth::new_note(200);
    }
    void TIM6_DAC_IRQHandler() noexcept {
        timer_6.clear_interrupt_flag();
        Synth::play_next_sample();
    }
    void TIM7_IRQHandler() noexcept {
        envelope_timer.clear_interrupt_flag();
        Synth::advance_envelope();
    }
}
