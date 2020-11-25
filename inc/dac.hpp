#ifndef DAC_HPP
#define DAC_HPP

namespace Dac {
    enum class DacNumber { Dac1, Dac2 };
    enum class ChannelSelection{ Tim6, Tim3or8, Tim7, Tim15, Tim2, Tim4, Exti9, Software };
    struct Config {
        DacNumber dac_num;
        ChannelSelection channel_select;
        bool buffer_disable;
        bool DMA_enable;
    };
    
    void enable_dac(Config const& cfg) noexcept;
}
#endif
