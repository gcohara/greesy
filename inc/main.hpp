#ifndef MAIN_HPP
#define MAIN_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <array>

enum class PeripheralStatus { ON, OFF };
using reg32 = std::uint32_t volatile * const;

// Constants
std::size_t constexpr sine_sample_length{ 256 };
std::size_t constexpr clock_speed{ 72'000'000 };
std::size_t constexpr desired_sample_rate{ 44'100 };
std::size_t constexpr autoreload_value{ clock_speed / desired_sample_rate };
std::size_t constexpr envelope_prescale{ 500 };
std::size_t constexpr midi_buffer_size{ 1024 };
std::float_t constexpr base_frequency {
                           static_cast<float>(desired_sample_rate)
                           /
                           sine_sample_length
};

// Circular buffer. Has a memory leak, and doesn't do anything when data is
// overwritten, but fine for our purposes
template <typename T, std::size_t size>
class CircularBuffer {
    T buffer[size];
    std::size_t read_index{ 0 };
    std::size_t write_index{ 0 };
    std::size_t elements{ 0 };
    std::size_t const capacity{ size };
    
public:
    // Flag to tell us when the write operation overwrites data that hasn't yet
    // been read
    bool data_loss{ false };
    // Assumes buffer isn't empty
    T read() noexcept {
        auto const out{ buffer[read_index] };
        read_index++;
        read_index %= capacity;
        if (elements > 0) { elements--; }
        return out;
    }
    
    void write(T data) noexcept {
        buffer[write_index] = data;
        write_index++;
        write_index %= capacity;
        if (elements < capacity) { elements++; }
        else { data_loss = true; }
    }
    
    bool buffer_empty() noexcept {
        return elements == 0;
    }

    std::size_t element_count() noexcept {
        return elements;
    }
    
};

#endif
