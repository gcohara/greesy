#ifndef DAC_HPP
#define DAC_HPP

namespace DAC {
    enum class DACNumber { DAC1, DAC2 };
    enum class ChannelSelection{ Tim6, Tim3or8, Tim7, Tim15, Tim2, Tim4, Exti9, Software };
    struct Config {
        DACNumber dac_num;
        ChannelSelection channel_select;
        bool buffer_disable;
        bool DMA_enable;
        bool trigger_enable;
    };
    
    void enable_dac(Config const& cfg) noexcept;
    // void
}

#endif
