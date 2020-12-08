#ifndef TIMERS_HPP
#define TIMERS_HPP

#include "interrupts.hpp"
#include "../main.hpp"
#include "rcc.hpp"
#include <cstdint>

/* TODO:
   Complete that enable interrupt method.
*/

namespace BasicTimers {
    enum class TimerNumber { Tim6, Tim7 };
    enum class MasterModeSelection { Reset, Enable, Update };
    using OnePulse = bool;
    using UpdateInterrupt = bool;
    using PrescaleValue = std::uint16_t;
    using AutoreloadValue = std::uint16_t;

    struct Config {
        MasterModeSelection master_mode;
        OnePulse onepulse;
        PrescaleValue psc_val;
        AutoreloadValue autorel_val;
        UpdateInterrupt interrupt;
    };
    
    template<TimerNumber tim_num>
    class BasicTimer {
        reg32 base_address{
            tim_num == TimerNumber::Tim6 ?
            (reg32) 0x40001000 : (reg32) 0x40001400
        };
        reg32 control1{ base_address };
        reg32 control2{ base_address + 0x1 };
        reg32 dma_interrupt_enable{ base_address + 0x3 };
        reg32 status_register{ base_address + 0x4 };
        reg32 prescaler{ base_address + 0xA };
        reg32 autoreload{ base_address + 0xB };
    public:
        void enable(Config const& cfg) noexcept {
            if constexpr(tim_num == TimerNumber::Tim6){
                RCC::enable_apb1_clock(RCC::APB1Peripheral::TIM6);
            }
            else if constexpr(tim_num == TimerNumber::Tim7){
                RCC::enable_apb1_clock(RCC::APB1Peripheral::TIM7);
            }
            *control1 |= (cfg.onepulse << 3);
            *dma_interrupt_enable |= cfg.interrupt;
            *control2 |= (static_cast<int>(cfg.master_mode) << 4);
            *prescaler = cfg.psc_val;
            *autoreload = cfg.autorel_val;
            // enable the timer
            *control1 |= 1;
        }
        void clear_interrupt_flag() noexcept {
            *status_register &= 0;
        }
        void enable_interrupt(Interrupts::InterruptPriority const priority) noexcept {
            
        }
    };
}

#endif
