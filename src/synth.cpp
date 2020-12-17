#include "../inc/synth.hpp"
#include "../inc/main.hpp"
#include "../inc/tables.hpp"
#include <cmath>
#include <cstdint>

std::float_t const extern base_frequency;

namespace {
    reg32 output_register{ nullptr };
    // global variables for sound 
    std::float_t volatile amplitude{ 1.0f };
    std::size_t volatile envelope_index{ 0 };
    std::float_t envelope_increment{ 0.01f };
    std::float_t volatile next_freq{ 100.0f };
    std::float_t volatile wavetable_increment{ next_freq / base_frequency };
    bool static volatile trigger_flag{ false };
    
    /* These two contants are used to decide whether we are close enough to a
     * zero-crossing to trigger a new note or not.
     * Midpoint is the zero point, and delta is the maximum distance from this
     * zero point that is considered close enough.
     */
    std::uint16_t constexpr midpoint{ 2048 };
    std::uint16_t constexpr delta{ 100 };
    
    void trigger_envelope() noexcept {
        trigger_flag = false;
        envelope_index = 0;
        amplitude = decay_envelope[envelope_index];
    }

    void play_frequency() noexcept {
        wavetable_increment = next_freq / base_frequency;
    }

    bool output_close_to_zero_crossing(std::float_t const output) noexcept {
        return output <= (midpoint + delta) && output >= (midpoint - delta);
    }

    std::float_t next_output_raw(std::float_t wavetable_index) noexcept {
        std::float_t integer_part;
        std::float_t fractional_part{ std::modf(wavetable_index, &integer_part) };
        std::size_t i = static_cast<std::size_t>(integer_part);
        // linear interpolation
        auto const raw_output {
            fractional_part * static_cast<std::float_t>(sine_wave[(i + 1) % 256] - sine_wave[i])
            + static_cast<std::float_t>(sine_wave[i])
        };
        return raw_output;
    }
}

namespace Synth {
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

    void new_note(std::uint8_t midi_note_number) noexcept {
        new_note(note_number_to_freq[midi_note_number]);
    }

    void stop_note() noexcept {
        envelope_index = envelope_length - 1;
        amplitude = 0;
        trigger_flag = false;
    }
    
    void play_next_sample() noexcept {
        std::float_t static wavetable_index{ 0.0f };
        auto const raw_output = next_output_raw(wavetable_index);
        
        // If we're close to zero, then we can retrigger the envelope
        if (trigger_flag && output_close_to_zero_crossing(raw_output)) {
            trigger_envelope();
            play_frequency();
        }
        
        // apply envelope
        auto const output = ((raw_output - 2048.0f) * amplitude) + 2048.0f;
        DAC::output_12bit_rightaligned_data_channel1(static_cast<std::uint32_t>(output));
        
        // increment index
        wavetable_index += wavetable_increment;
        if (static_cast<std::size_t>(wavetable_index) >= sine_sample_length) {
            wavetable_index -= sine_sample_length;
        }
    }
}
