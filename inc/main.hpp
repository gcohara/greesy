#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstddef>
#include <cstdint>

enum class PeripheralStatus { ON, OFF };
using reg32 = std::uint32_t volatile * const;

#endif
