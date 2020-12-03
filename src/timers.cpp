#include "../inc/main.hpp"
#include "../inc/timers.hpp"
#include "../inc/rcc.hpp"

namespace {
    struct BasicTimerRegisters{
        reg32 base;
        reg32 control1;
        reg32 control2;
        reg32 dma_interrupt_enable;
        reg32 status_register;
        reg32 prescaler;
        reg32 autoreload;
        BasicTimerRegisters(reg32 base_register):
            base{ base_register },
            control1{ base_register },
            control2{ base_register + 0x1 },
            dma_interrupt_enable{ base_register + 0x3 },
            status_register{ base_register + 0x4 },
            prescaler{ base_register + 0xA },
            autoreload{ base_register + 0xB }{}
        
    };
    BasicTimerRegisters Tim6Registers{ (reg32) 0x40001000 };
    BasicTimerRegisters Tim7Registers{ (reg32) 0x40001400 };

    BasicTimerRegisters const* const
    get_register_pointer
    (BasicTimers::TimerNumber const tim_num) noexcept {
        BasicTimerRegisters* timer_registers;
        if(tim_num == BasicTimers::TimerNumber::Tim6){
            RCC::enable_apb1_clock(RCC::APB1Peripheral::TIM6);
            timer_registers = &Tim6Registers;
        }
        else if(tim_num == BasicTimers::TimerNumber::Tim7){
            RCC::enable_apb1_clock(RCC::APB1Peripheral::TIM6);
            timer_registers = &Tim7Registers;
        }
        return timer_registers;
    }
}

namespace BasicTimers {
    // currently only works for timer 6 (but not using timer 7 yet)
    void enable_basic_timer(TimerNumber const tim_num, Config const& cfg) noexcept {
        auto timer_registers{ get_register_pointer(tim_num) };
        if(tim_num == TimerNumber::Tim6){
            RCC::enable_apb1_clock(RCC::APB1Peripheral::TIM6);
            timer_registers = &Tim6Registers;
        }
        else if(tim_num == TimerNumber::Tim7){
            RCC::enable_apb1_clock(RCC::APB1Peripheral::TIM6);
            timer_registers = &Tim7Registers;
        }
        *timer_registers->control1 |= (cfg.onepulse << 3);
        *timer_registers->dma_interrupt_enable |= cfg.interrupt;
        *timer_registers->control2 |= (static_cast<int>(cfg.master_mode) << 4);
        *timer_registers->prescaler = cfg.psc_val;
        *timer_registers->autoreload = cfg.autorel_val;
        // enable the timer
        *timer_registers->control1 |= 1;
        
    }
    // also currently only works for timer 6
    void clear_interrupt_flag(TimerNumber const tim_num) noexcept {
        auto timer_registers{ get_register_pointer(tim_num) };
        *timer_registers->status_register &= 0;
    }
}
