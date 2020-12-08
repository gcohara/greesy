
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
#include <cmath>

GPIO_PORT static portA{ GPIOPortLetter::A };
GPIO_PORT static portE{ GPIOPortLetter::E };
GPIO_PIN static led{ portE, 10 };
GPIO_PIN static push_button{ portA, 0 };
GPIO_PIN static audio_out{ portA, 4 };
BasicTimers::BasicTimer<BasicTimers::TimerNumber::Tim6> static timer_6;
BasicTimers::BasicTimer<BasicTimers::TimerNumber::Tim7> static envelope_timer;

// global constants
// note that apb1 domain can't go higher than this, so while clock speed is
// actually 72mhz we set it to this
std::size_t constexpr clock_speed{ 72'000'000 };
std::size_t constexpr desired_sample_rate{ 44'100 };
std::size_t constexpr autoreload_value{ clock_speed / desired_sample_rate };
std::size_t constexpr envelope_length{ 128 };
std::size_t constexpr sine_sample_length{ 256 };
std::float_t constexpr base_frequency{ static_cast<float>(desired_sample_rate) / sine_sample_length };

// wavetables
using WaveSample = std::array<std::uint16_t, sine_sample_length>;
WaveSample constexpr static sine_wave{
    2047,2097,2147,2198,2248,2298,2347,2397,2446,2496,2545,2593,2641,2689,
    2737,2784,2831,2877,2922,2968,3012,3056,3100,3142,3185,3226,3267,3307,
    3346,3384,3422,3459,3495,3530,3564,3597,3630,3661,3692,3721,3749,3777,
    3803,3829,3853,3876,3898,3919,3939,3957,3975,3991,4006,4020,4033,4045,
    4055,4064,4072,4079,4085,4089,4092,4094,4095,4094,4092,4089,4085,4079,
    4072,4064,4055,4045,4033,4020,4006,3991,3975,3957,3939,3919,3898,3876,
    3853,3829,3803,3777,3749,3721,3692,3661,3630,3597,3564,3530,3495,3459,
    3422,3384,3346,3307,3267,3226,3185,3142,3100,3056,3012,2968,2922,2877,
    2831,2784,2737,2689,2641,2593,2545,2496,2446,2397,2347,2298,2248,2198,
    2147,2097,2047,1997,1947,1896,1846,1796,1747,1697,1648,1598,1549,1501,
    1453,1405,1357,1310,1263,1217,1172,1126,1082,1038, 994, 952, 909, 868,
    827, 787, 748, 710, 672, 635, 599, 564, 530, 497, 464, 433, 402, 373,
    345, 317, 291, 265, 241, 218, 196, 175, 155, 137, 119, 103,  88,  74,
    61,  49,  39,  30,  22,  15,   9,   5,   2,   0,   0,   0,   2,   5,
    9,  15,  22,  30,  39,  49,  61,  74,  88, 103, 119, 137, 155, 175,
    196, 218, 241, 265, 291, 317, 345, 373, 402, 433, 464, 497, 530, 564,
    599, 635, 672, 710, 748, 787, 827, 868, 909, 952, 994,1038,1082,1126,
    1172,1217,1263,1310,1357,1405,1453,1501,1549,1598,1648,1697,1747,1796,
    1846,1896,1947,1997
};
// global variables

std::float_t volatile amplitude{ 1.0f };
std::size_t volatile envelope_index{ 0 };
// std::float_t envelope_increment{ 0.01f };
std::float_t volatile wavetable_index{ 0.0f };
std::float_t volatile freq{ 100.0f };
std::float_t volatile wavetable_increment{ freq / base_frequency };

// std::float_t volatile decay_envelope[envelope_length] {
//     1.000000,0.98066,0.96169,0.94309,0.92485,0.90696,0.88942,0.87222,0.85535,
//     0.83880,0.82258,0.80667,0.79107,0.77576,0.76076,0.74605,0.73162,0.71746,0.70359,
//     0.68998,0.67663,0.66355,0.65071,0.63813,0.62578,0.61368,0.60181,0.59017,0.57876,
//     0.56756,0.55658,0.54582,0.53526,0.52491,0.51476,0.50480,0.49504,0.48546,0.47607,
//     0.46686,0.45783,0.44898,0.44029,0.43178,0.42343,0.41524,0.40721,0.39933,0.39161,
//     0.38403,0.37660,0.36932,0.36218,0.35517,0.34830,0.34156,0.33496,0.32848,0.32213,
//     0.31590,0.30979,0.30379,0.29792,0.29216,0.28650,0.28096,0.27553,0.27020,0.26497,
//     0.25985,0.25482,0.24989,0.24506,0.24032,0.23567,0.23111,0.22664,0.22226,0.21796,
//     0.21375,0.20961,0.20556,0.20158,0.19768,0.19386,0.19011,0.18643,0.18283,0.17929,
//     0.17582,0.17242,0.16909,0.16582,0.16261,0.15946,0.15638,0.15335,0.15039,0.14748,
//     0.14463,0.14183,0.13909,0.13640,0.13376,0.13117,0.12863,0.12615,0.12371,0.12131,
//     0.11897,0.11667,0.11441,0.11220,0.11003,0.10790,0.10581,0.10377,0.10176,0.09979,
//     0.09786,0.09597,0.09411,0.09229,0.09051,0.08876,0.08704,0.08535,0.08370,0.08208,
//     0.08050,0.07894,0.07741,0.07592,0.07445,0.07301,0.07160,0.07021,0.06885,0.06752,
//     0.06622,0.06493,0.06368,0.06245,0.06124,0.06005,0.05889,0.05775,0.05664,0.05554,
//     0.05447,0.05341,0.05238,0.05137,0.05037,0.04940,0.04844,0.04751,0.04659,0.04569,
//     0.04480,0.04394,0.04309,0.04225,0.04144,0.04064,0.03985,0.03908,0.03832,0.03758,
//     0.03685,0.03614,0.03544,0.03476,0.03408,0.03343,0.03278,0.03214,0.03152,0.03091,
//     0.03032,0.02973,0.02915,0.02859,0.02804,0.02750,0.02696,0.02644,0.02593,0.02543,
//     0.02494,0.02445,0.02398,0.02352,0.02306,0.02262,0.02218,0.02175,0.02133,0.02092,
//     0.02051,0.02012,0.01973,0.01935,0.01897,0.01860,0.01824,0.01789,0.01755,0.01721,
//     0.01687,0.01655,0.01623,0.01591,0.01561,0.01530,0.01501,0.01472,0.01443,0.01415,
//     0.01388,0.01361,0.01335,0.01309,0.01284,0.01259,0.01234,0.01211,0.01187,0.01164,
//     0.01142,0.01120,0.01098,0.01077,0.01056,0.01035,0.01015,0.00996,0.00977,0.00958,
//     0.00939,0.00921,0.00903,0.00886,0.00869,0.00852,0.00835,0.00819,0.00803,0.00788,
//     0.00773,0.00758,0.00743,0.00729,0.00714,0.00701,0
// };

std::float_t volatile decay_envelope[envelope_length];

auto main() -> int {
    for (auto& x : decay_envelope) {
        x = 1.0f;
    }
    decay_envelope[envelope_length - 1] = 0.0f;
    RCC::set_to_72Mhz();        // gotta go fast
    // Define our configs for the peripherals we will be using
    DAC::Config constexpr dac1_cfg{
        .dac_num{ DAC::DACNumber::DAC1 },
        .buffer_disable{ true },
        .trigger_enable{ false },
    };
    BasicTimers::Config constexpr tim6_cfg{
        .psc_val{ 0 },
        .autorel_val{ autoreload_value },
        .interrupt{ true }
    };
    BasicTimers::Config constexpr envelope_timer_config{
        .psc_val{ 5000 },
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
    // declarations at the top!
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
        led.toggle_output();
        freq += 100;
        play_frequency(freq);
        trigger_flag = true;
    }
    void TIM6_DAC_IRQHandler() noexcept {
        timer_6.clear_interrupt_flag();
        tim6_handler();
        led.toggle_output();

    }
    void TIM7_IRQHandler() noexcept {
        envelope_timer.clear_interrupt_flag();
        tim7_handler();
    }
}


