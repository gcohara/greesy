#include "../inc/dac.hpp"
#include "../inc/rcc.hpp"

namespace {
    reg32 dac_base{ (reg32) 0x40007400 };
    reg32 control_register{ dac_base };
    
    void enable_clock(Dac::DacNumber dacnum) noexcept {
        if (dacnum == Dac::DacNumber::Dac1) { Rcc::enable_apb1_clock(Rcc::APB1Peripheral::DAC1); }
        else { Rcc::enable_apb1_clock(Rcc::APB1Peripheral::DAC2); }
    }
}

namespace Dac {
    void enable_dac(Config const& cfg) noexcept {
        enable_clock(cfg.dac_num);
        auto const bit_offset{ 16 * static_cast<int>(cfg.dac_num) };
        // would be quicker to be this in one step but makes code much worse to read
        *control_register |= (cfg.DMA_enable << (bit_offset + 12));
        *control_register |= (cfg.buffer_disable << (bit_offset + 1));
        *control_register |= (static_cast<int>(cfg.channel_select) << (bit_offset + 3));
        *control_register |= (1 << (bit_offset));
    }
}
