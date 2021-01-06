#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "main.hpp"
#include "peripherals/dac.hpp"
#include <cmath>
#include <cstdint>

namespace Synth {
    void play_next_sample() noexcept;
    void advance_envelope() noexcept;
    void new_note(std::float_t new_freq) noexcept;
    void new_note(std::uint8_t midi_note_number) noexcept;
    void stop_note(std::uint8_t midi_note_number) noexcept;
    void set_output_register() noexcept;
}

#endif
