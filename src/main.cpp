#include "../inc/main.hpp"
#include "../inc/gpio.hpp"
#include "../inc/sysconfig.hpp"
#include "../inc/interrupts.hpp"
#include "../inc/timers.hpp"
#include "../inc/dac.hpp"
#include "../inc/dma.hpp"
#include "../inc/rcc.hpp"
#include <cstddef>
#include <cstdint>
#include <array>

GPIO_PORT static portA{ GPIOPortLetter::A };
GPIO_PORT static portE{ GPIOPortLetter::E };
GPIO_PIN static pin_e_10{ portE, 10 };
GPIO_PIN static pin_a_0{ portA, 0 };
GPIO_PIN static pin_a_4{ portA, 4 };
// note that apb1 domain can't go higher than this, so while clock speed is
// actually 72mhz we set it to this
std::size_t constexpr clock_speed{ 36'000'000 };
std::size_t constexpr desired_sample_rate{ 44'100 };
std::size_t constexpr autoreload_value{ clock_speed / desired_sample_rate };
std::size_t constexpr sine_sample_length{ 256 };
std::size_t constexpr base_frequency{ desired_sample_rate / sine_sample_length };
float wavetable_increment{ 1.301 };


std::array<std::uint16_t, sine_sample_length> constexpr static sine_wave{
    2047,2097,2147,2198,2248,2298,2347,2397,2446,2496,2545,2593,2641,2689,2737,2784,2831,2877,2922,2968,
        3012,3056,3100,3142,3185,3226,3267,3307,3346,3384,3422,3459,3495,3530,3564,3597,3630,3661,3692,3721,
        3749,3777,3803,3829,3853,3876,3898,3919,3939,3957,3975,3991,4006,4020,4033,4045,4055,4064,4072,4079,
        4085,4089,4092,4094,4095,4094,4092,4089,4085,4079,4072,4064,4055,4045,4033,4020,4006,3991,3975,3957,
        3939,3919,3898,3876,3853,3829,3803,3777,3749,3721,3692,3661,3630,3597,3564,3530,3495,3459,3422,3384,
        3346,3307,3267,3226,3185,3142,3100,3056,3012,2968,2922,2877,2831,2784,2737,2689,2641,2593,2545,2496,
        2446,2397,2347,2298,2248,2198,2147,2097,2047,1997,1947,1896,1846,1796,1747,1697,1648,1598,1549,1501,
        1453,1405,1357,1310,1263,1217,1172,1126,1082,1038,994,952,909,868,827,787,748,710,672,635,
        599,564,530,497,464,433,402,373,345,317,291,265,241,218,196,175,155,137,119,103,
        88,74,61,49,39,30,22,15,9,5,2,0,0,0,2,5,9,15,22,30,
        39,49,61,74,88,103,119,137,155,175,196,218,241,265,291,317,345,373,402,433,
        464,497,530,564,599,635,672,710,748,787,827,868,909,952,994,1038,1082,1126,1172,1217,
        1263,1310,1357,1405,1453,1501,1549,1598,1648,1697,1747,1796,1846,1896,1947,1997
        };


auto main() -> int {
    pin_e_10.initialise_gp_output();
    pin_a_0.initialise_gp_input(GPIOInputPUPD::PushDown);
    pin_a_4.initialise_analogue();
    RCC::set_to_72Mhz();
    
    
    Interrupts::enable_gpio_interrupt(pin_a_0, {1, true, false});
    DAC::Config constexpr dac1_cfg{
        .dac_num{ DAC::DACNumber::DAC1 },
            .buffer_disable{ true },
            .trigger_enable{ false },
    };
    DAC::enable_dac(dac1_cfg);
    
    
    BasicTimers::Config constexpr tim6_cfg{
        .psc_val{ 0 },
            .autorel_val{ autoreload_value },
            .interrupt{ true }
    };
    BasicTimers::enable_basic_timer(BasicTimers::TimerNumber::Tim6, tim6_cfg);
    Interrupts::enable_timer6_interrupt(0);
    
    while (1) {
        
    }
}

#include <cmath>

void inline tim6_handler() noexcept {
    // many problems with this function, relies on overflow of 8bit int
    // direct register access too
    float_t static raw_index{ 0 };
    std::uint8_t static j = 0;
    
    if (static_cast<std::size_t>(raw_index) >= sine_sample_length) {
        raw_index -= sine_sample_length;
    }
    float_t integer_part;
    float_t fractional_part{ std::modf(raw_index, &integer_part) };
    
    std::size_t i = static_cast<std::size_t>(integer_part);
    

    auto output {
        fractional_part * static_cast<float_t>(sine_wave[(i + 1) % 256] - sine_wave[i])
        + static_cast<float_t>(sine_wave[i])
    };
    
    (* (uint32_t *) 0x40007408) = static_cast<std::uint32_t>(output);
    // if ((unsigned) raw_index > sine_sample_length - 1) {
    //     raw_index-= sine_sample_length;
    // }
    raw_index += wavetable_increment;
    j++;
    // raw_index++;
}



extern "C" {
    void EXTI0_IRQHandler() noexcept {
        Interrupts::clear_gpio_interrupt_flag(0);
        pin_e_10.toggle_output();
        if (wavetable_increment > 3) { wavetable_increment = 0; }
        else { wavetable_increment++; }
    }
    void TIM6_DAC_IRQHandler() noexcept {
        BasicTimers::clear_interrupt_flag(BasicTimers::TimerNumber::Tim6);
        tim6_handler();
    }
}


