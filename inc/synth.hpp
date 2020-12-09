#ifndef SYNTH_HPP
#define SYNTH_HPP

#include "peripherals/dac.hpp"
#include <cmath>

namespace Synth {
    void play_next_sample() noexcept;
    void advance_envelope() noexcept;
    void change_frequency(std::float_t freq) noexcept;
    void trigger_envelope() noexcept;
    void set_output_register() noexcept;
}

#endif
