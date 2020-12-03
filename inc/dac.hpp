#ifndef DAC_HPP
#define DAC_HPP

#include <cstdint>
namespace DAC {
    // Redundant, we only have one DAC on this chip!
    enum class DACNumber { DAC1, DAC2 };
    enum class ChannelSelect { Channel1, Channel2 };
    enum class DataFormat { Right12bit, Left12bit, Right8bit };
    enum class TriggerSelection{ Tim6, Tim3or8, Tim7, Tim15, Tim2, Tim4, Exti9, Software };
    using Data = std::uint16_t;
    struct Config {
        DACNumber dac_num;
        TriggerSelection trigger_select;
        bool buffer_disable;
        bool DMA_enable;
        bool trigger_enable;
    };
    
    void enable_dac(Config const& cfg) noexcept;
    void output_12bit_rightaligned_data_channel1(Data data) noexcept;
}

#endif
