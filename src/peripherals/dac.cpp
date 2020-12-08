#include "../../inc/peripherals/dac.hpp"
#include "../../inc/peripherals/rcc.hpp"
namespace{
    namespace DACAnon {
        reg32 dac_base{ (reg32) 0x40007400 };
        reg32 control_register{ dac_base };
        reg32 channel1_12bit_right_holding{ dac_base + 0x2 };
        reg32 channel1_12bit_left_holding{ dac_base + 0x3 };
        reg32 channel1_8bit_right_holding{ dac_base + 0x4 };
        
        void enable_clock() noexcept {
            RCC::enable_apb1_clock(RCC::APB1Peripheral::DAC1);
        }
    }
}

namespace DAC {
    using namespace DACAnon;
    
    void enable_dac(Config const& cfg) noexcept {
        enable_clock();
        // upper 16 bits and lower 16 bits of the configuration register are
        // identical, except upper configures channel 2 while lower configures
        // channel 1 - so we get an offset dependent on the channel
        auto const config_register_offset{ static_cast<int>(cfg.chan_num) ? 0 : 16 };
        // would be quicker to be this in one step but makes code much worse to
        // read and the compiler will probably optimise it anyway
        *control_register |= (cfg.DMA_enable << (config_register_offset + 12));
        *control_register |= (cfg.buffer_disable << (config_register_offset + 1));
        *control_register |= (static_cast<int>(cfg.trigger_select) << (config_register_offset + 3));
        *control_register |= (1 << cfg.trigger_enable);
        // enable the dac
        *control_register |= (1 << (config_register_offset));
    }
    
    void output_12bit_rightaligned_data_channel1(Data data) noexcept {
        *channel1_12bit_right_holding = data;
    }
}
