#ifndef USART_HPP
#define USART_HPP
#include <cstdint>
#include "../main.hpp"
#include "rcc.hpp"

namespace {
}

namespace USART {
    std::uint32_t unit_test_registers[11]{};
    enum class USARTNumber{ USART1, USART2, USART3, UART4, UART5, UnitTest };
    enum class WordLength{ Seven, Eight, Nine };
    enum class StopBits{ One, Half, Two, OneAndHalf };
    struct Config {
        WordLength word_len;
        StopBits stop_bits;
        std::uint32_t baud_rate;
        bool receive_enable;
        bool transmit_enable;
        bool receive_DMA;
        bool transmit_DMA;
        bool auto_baud;
        bool enable_rxne_interrupt;
    };

    template<USARTNumber usart_num>
    class USART {
        reg32 base_address{
            usart_num == USARTNumber::USART1 ? (reg32) 0x40013800 :
            usart_num == USARTNumber::USART2 ? (reg32) 0x40004400 :
            usart_num == USARTNumber::USART3 ? (reg32) 0x40004800 :
            usart_num == USARTNumber::UART4 ? (reg32) 0x40004C00 :
            usart_num == USARTNumber::UART5 ? (reg32) 0x40005000 :
            unit_test_registers 
        };
        reg32 control1{ base_address };
        reg32 control2{ base_address + 0x1 };
        reg32 control3{ base_address + 0x2 };
        reg32 baud_rate_register{ base_address + 0x3 };
        reg32 request_register{ base_address + 0x8 };
        reg32 received_data{ base_address + 0x9 };
    public:
        void enable(Config const& cfg) noexcept {
            if constexpr(usart_num == USARTNumber::USART1) {
                RCC::enable_apb2_clock(RCC::APB2Peripheral::USART1);
            }
            else if constexpr(usart_num == USARTNumber::USART2) {
                RCC::enable_apb1_clock(RCC::APB1Peripheral::USART2);
            }
            else if constexpr(usart_num == USARTNumber::USART3) {
                RCC::enable_apb1_clock(RCC::APB1Peripheral::USART3);
            }
            else if constexpr(usart_num == USARTNumber::UART4) {
                RCC::enable_apb1_clock(RCC::APB1Peripheral::UART4);
            }
            else if constexpr(usart_num == USARTNumber::UART5) {
                RCC::enable_apb1_clock(RCC::APB1Peripheral::UART5);
            }
            // Clear any previous config
            *control1 = 0;
            *control2 = 0;
            *control3 = 0;
            // Set word length
            *control1 |= (((cfg.word_len == WordLength::Seven) << 28)
                          | ((cfg.word_len == WordLength::Nine) << 12));
            // Set interrupt enable
            *control1 |= (cfg.enable_rxne_interrupt << 5);
            // Set stop bit length
            *control2 |= (static_cast<std::uint32_t>(cfg.stop_bits) << 12);
            // Enable DMA if necessary
            *control3 |= ((cfg.receive_DMA << 6) | (cfg.transmit_DMA << 7));
            if (cfg.auto_baud) {
                *control2 |= (1 << 20);
            }
            else {
                // Set baud rate divider
                *baud_rate_register = clock_speed / cfg.baud_rate;
            }
            // Enable the peripheral
            *control1 |= (1 | cfg.receive_enable << 2 | cfg.transmit_enable << 3);
        }

        std::uint8_t read_data() noexcept {
            auto volatile data{ *received_data };
            *request_register &= (1 << 3);
            return data;
        }
    };
    
}

#endif
