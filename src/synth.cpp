#include "../inc/synth.hpp"
#include "../inc/main.hpp"
#include "../inc/tables.hpp"
#include <cmath>

std::float_t extern base_frequency;

namespace {
    reg32 output_register{ nullptr };
    // global variables for sound 
    std::float_t volatile amplitude{ 1.0f };
    std::size_t volatile envelope_index{ 0 };
    std::float_t envelope_increment{ 0.01f };
    std::float_t volatile wavetable_index{ 0.0f };
    std::float_t volatile next_freq{ 100.0f };
    std::float_t volatile wavetable_increment{ next_freq / base_frequency };
    bool static volatile trigger_flag{ false };
    std::uint16_t constexpr midpoint{ 2048 };
    std::uint16_t constexpr delta{ 100 };
    
    void trigger_envelope() noexcept {
        trigger_flag = false;
        envelope_index = 0;
        amplitude = decay_envelope[envelope_index];
    }

    void play_frequency() {
        wavetable_increment = next_freq / base_frequency;
    }
}

namespace Synth {
    // These need moving, ideally spinning off into a synth.cpp module
    
    void advance_envelope() noexcept {
        amplitude = decay_envelope[envelope_index];
        if (envelope_index < envelope_length - 1) {
            envelope_index++;
        }
    }

    void new_note(std::float_t freq) noexcept {
        next_freq = freq;
        trigger_flag = true;
    }
    
    
    void  play_next_sample() noexcept {
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
        // If we're closed to zero, then we can retrigger the envelope
        // Potentially spin off into retrigger_envelope?
        // Rename trigger_envelope to new_note()?
        // Put the condition in a variable called 'close to zero crossing'
        auto const close_to_zero_crossing{
            trigger_flag && output <= (midpoint + delta) && output >= (midpoint - delta)
        };
        if (close_to_zero_crossing) {
            trigger_envelope();
            play_frequency();
        }
        // apply envelope
        output = (output - 2048.0f) * amplitude;
        output += 2048.0f;
        DAC::output_12bit_rightaligned_data_channel1(static_cast<std::uint32_t>(output));
        wavetable_index += wavetable_increment;
    }
}
