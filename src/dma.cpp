#include "../inc/dma.hpp"
#include "../inc/rcc.hpp"
#include <cstdint>

namespace {
    // this is now for dma1
    reg32 dma2_base{ (reg32) 0x40020400 };
    reg32 dma2_channel3_config{ dma2_base + 0xC };
    reg32 dma2_channel3_transfer_length{ dma2_base + 0xD };
    reg32 dma2_channel3_periph_address{ dma2_base + 0xE };
    reg32 dma2_channel3_memory_address{ dma2_base + 0xF };
    reg32 dac_channel1_12bit_right_data_hold_register{ (reg32) 0x4000740C };
}

namespace DMA {
    void enable_dac_dma
    (DAC::DACNumber dac_num, reg32 memory_address, DataToSendAmount to_send) noexcept {
        // only works for dac1 as it stands
        RCC::enable_ahb_clock(RCC::AHBPeripheral::DMA2);
        *dma2_channel3_transfer_length = to_send;
        *dma2_channel3_periph_address =
            reinterpret_cast<std::uintptr_t>(dac_channel1_12bit_right_data_hold_register);
        *dma2_channel3_memory_address =
            reinterpret_cast<std::uintptr_t>(memory_address);
        // Set memory size and peripheral size to 16 bits
        *dma2_channel3_config |= (1 << 10);
        *dma2_channel3_config |= (1 << 8);
        // Set priority to very high
        *dma2_channel3_config |= (3 << 12);
        // Enabled memory increment mode
        *dma2_channel3_config |= (1 << 7);
        // Enable circular mode
        *dma2_channel3_config |= (1 << 5);
        // Set to read from memory to peripheral
        *dma2_channel3_config |= (1 << 4);
        // Finally, enable DMA
        *dma2_channel3_config |= 1;
    }
}
