#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstddef>
#include <cstdint>

enum class PeripheralStatus { ON, OFF };
using reg32 = std::uint32_t * const;

template <std::size_t MIN, std::size_t MAX>
struct RestrictedInteger {
    int value;
    template <std::size_t val>
    constexpr RestrictedInteger() {
        auto const within_range{ (val >= MIN) && (val <= MAX) };
        static_assert(within_range);
        value = val;
    }
};

reg32 RCC_CLOCK_ENABLE_AHB{ (reg32) 0x40021014 };


#endif
