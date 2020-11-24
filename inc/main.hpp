#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdint>

enum class PeripheralStatus { ON, OFF };
using reg32 = std::uint32_t * const;

reg32 RCC_CLOCK_ENABLE_AHB{ (reg32) 0x40021014 };


#endif
