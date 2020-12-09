#include "../inc/main.hpp"
#include "../inc/peripherals/gpio.hpp"
#include "../inc/peripherals/sysconfig.hpp"
#include "../inc/peripherals/interrupts.hpp"
#include "../inc/peripherals/timers.hpp"
#include "../inc/peripherals/dac.hpp"
#include "../inc/peripherals/rcc.hpp"
#include "../inc/tables.hpp"
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
std::float_t constexpr base_frequency{ static_cast<float>(desired_sample_rate) / sine_sample_length };

// global variables for sound 
std::float_t volatile amplitude{ 1.0f };
std::size_t volatile envelope_index{ 0 };
// std::float_t envelope_increment{ 0.01f };
std::float_t volatile wavetable_index{ 0.0f };
std::float_t volatile freq{ 100.0f };
std::float_t volatile wavetable_increment{ freq / base_frequency };

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
    // BasicTimers::enable_basic_timer(BasicTimers::TimerNumber::Tim6, tim6_cfg);
    timer_6.enable(tim6_cfg);
    Interrupts::enable_timer6_interrupt(1);
    envelope_timer.enable(envelope_timer_config);
    Interrupts::enable_timer7_interrupt(0);
    
    while (1) {
        
    }
}


// These need moving, ideally spinning off into a synth.cpp module
bool static volatile trigger_flag{ false };
std::uint16_t constexpr midpoint{ 2048 };
std::uint16_t constexpr delta{ 100 };


void play_frequency(std::float_t freq) {
    wavetable_increment = freq / base_frequency;
}

void inline tim7_handler() noexcept {
    amplitude = decay_envelope[envelope_index];
    if (envelope_index < envelope_length - 1) {
        envelope_index++;
    }
}


void inline tim6_handler() noexcept {
    if (static_cast<std::size_t>(wavetable_index) >= sine_sample_length) {
        wavetable_index -= sine_sample_length;
        // retrigger envelope here toox
    }
    float_t integer_part;
    float_t fractional_part{ std::modf(wavetable_index, &integer_part) };
    std::size_t i = static_cast<std::size_t>(integer_part);
    // linear interpolation
    auto volatile output {
        fractional_part * static_cast<float_t>(sine_wave[(i + 1) % 256] - sine_wave[i])
        + static_cast<float_t>(sine_wave[i])
    };
    
    // calculate amplitude
    // if (static_cast<std::size_t>(envelope_index) >= envelope_length) {
    //     envelope_index -= envelope_length;
    // }
    // float_t integer_part_env;
    // float_t fractional_part_env{ std::modf(envelope_index, &integer_part_env) };
    // std::size_t j = static_cast<std::size_t>(integer_part_env);
    // linear interpolation
    // auto amplitude {
    //     fractional_part_env * static_cast<float_t>(decay_envelope[(j + 1) % envelope_length] - decay_envelope[j])
    //     + static_cast<float_t>(decay_envelope[j])
    // };
    
    if (trigger_flag && output <= (midpoint + delta) && output >= (midpoint - delta)) {
        trigger_flag = false;
        envelope_index = 0;
        amplitude = decay_envelope[envelope_index];
    }
    
    
    // apply envelope
    output = (output - 2048.0f) * amplitude;
    output += 2048.0f;
    DAC::output_12bit_rightaligned_data_channel1(static_cast<std::uint32_t>(output));
    wavetable_index += wavetable_increment;
    // if (envelope_index < envelope_length - envelope_increment - 1){
    // envelope_index += envelope_increment;
}



extern "C" {
    void EXTI0_IRQHandler() noexcept {
        Interrupts::clear_gpio_interrupt_flag(0);
        freq += 100;
        play_frequency(freq);
        trigger_flag = true;
    }
    void TIM6_DAC_IRQHandler() noexcept {
        timer_6.clear_interrupt_flag();
        tim6_handler();
    }
    void TIM7_IRQHandler() noexcept {
        envelope_timer.clear_interrupt_flag();
        tim7_handler();
    }
}
