#ifndef MAIN_HPP
#define MAIN_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>

enum class PeripheralStatus { ON, OFF };
using reg32 = std::uint32_t volatile * const;

// Constants
std::size_t constexpr sine_sample_length{ 256 };
std::size_t constexpr clock_speed{ 72'000'000 };
std::size_t constexpr desired_sample_rate{ 44'100 };
std::size_t constexpr autoreload_value{ clock_speed / desired_sample_rate };
std::size_t constexpr envelope_prescale{ 500 };
std::float_t constexpr base_frequency {
                           static_cast<float>(desired_sample_rate)
                           /
                           sine_sample_length
};

#endif
