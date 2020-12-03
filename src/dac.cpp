#include "../inc/dac.hpp"
#include "../inc/rcc.hpp"

namespace {
    reg32 dac_base{ (reg32) 0x40007400 };
    reg32 control_register{ dac_base };
    reg32 channel1_12bit_right_holding{ dac_base + 0x2 };
    reg32 channel1_12bit_left_holding{ dac_base + 0x3 };
    reg32 channel1_8bit_right_holding{ dac_base + 0x4 };
    
    void enable_clock(DAC::DACNumber dacnum) noexcept {
        if (dacnum == DAC::DACNumber::DAC1) {
            RCC::enable_apb1_clock(RCC::APB1Peripheral::DAC1);
        }
        else {
            RCC::enable_apb1_clock(RCC::APB1Peripheral::DAC2);
        }
    }
}

namespace DAC {
    void enable_dac(Config const& cfg) noexcept {
        enable_clock(cfg.dac_num);
        auto const bit_offset{ 16 * static_cast<int>(cfg.dac_num) };
        // would be quicker to be this in one step but makes code much worse to read
        *control_register |= (cfg.DMA_enable << (bit_offset + 12));
        *control_register |= (cfg.buffer_disable << (bit_offset + 1));
        *control_register |= (static_cast<int>(cfg.trigger_select) << (bit_offset + 3));
        *control_register |= (1 << (bit_offset));
        *control_register |= (1 << cfg.trigger_enable);
    }

    void output_12bit_rightaligned_data_channel1(Data data) noexcept {
        *channel1_12bit_right_holding = data;
    }
}
