#ifndef DMA_HPP
#define DMA_HPP

#include <cstdint>

#include "../inc/dac.hpp"
#include "../inc/main.hpp"

namespace DMA {
    enum class DMAPriority { Low, Medium, High, VeryHigh };
    enum class MemorySize { OneByte, TwoBytes, FourBytes };
    enum class PeripheralSize { OneByte, TwoBytes, FourBytes };
    enum class TransferDirection { PeriphToMemory, MemoryToPeriph, };
    using MemoryIncrementEnable = bool;
    using PeripheralIncrementEnable = bool;
    using CircularModeEnable = bool;
    using DataToSendAmount = std::uint16_t;

    // enables DAC1/DAC2 DMA via DMA2 on channels 3/4 respectively
    // uses 'sane' defaults for this project
    void enable_dac_dma
    (DAC::DACNumber dac_num, reg32 memory_address, DataToSendAmount to_send) noexcept;
}

#endif
