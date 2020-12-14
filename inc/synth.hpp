#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "main.hpp"
#include "peripherals/dac.hpp"
#include <cmath>

namespace Synth {
    void play_next_sample() noexcept;
    void advance_envelope() noexcept;
    void new_note(std::float_t new_freq) noexcept;
    void set_output_register() noexcept;
    int add_function(int a, int b);
}

#endif
