#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "peripherals/dac.hpp"
#include <cmath>

namespace Synth {
    void play_next_sample() noexcept;
    void advance_envelope() noexcept;
    void new_note(std::float_t new_freq) noexcept;
    void set_output_register() noexcept;
}

#endif
