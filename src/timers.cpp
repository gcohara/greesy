#include "../inc/main.hpp"
#include "../inc/timers.hpp"
#include "../inc/rcc.hpp"

namespace {
    reg32 tim6_base{ (reg32) 0x40001000 };
    reg32 tim6_control1{ tim6_base };
    reg32 tim6_control2{ tim6_base + 0x1 };
    reg32 tim6_dma_interrupt_enable{ tim6_base + 0x3 };
    reg32 tim6_status_register{ tim6_base + 0x4 };
    reg32 tim6_prescaler{ tim6_base + 0xA };
    reg32 tim6_autoreload{ tim6_base + 0xB };
}

namespace BasicTimers {
    // currently only works for timer 6 (but not using timer 7 yet)
    void enable_basic_timer(TimerNumber const tim_num, Config const& cfg) noexcept {
        RCC::enable_apb1_clock(RCC::APB1Peripheral::TIM6);
        *tim6_control1 |= (cfg.onepulse << 3);
        *tim6_dma_interrupt_enable |= cfg.interrupt;
        *tim6_control2 |= (static_cast<int>(cfg.master_mode) << 4);
        *tim6_prescaler = cfg.psc_val;
        *tim6_autoreload = cfg.autorel_val;
        // enable the timer
        *tim6_control1 |= 1;
    }
    // also currently only works for timer 6
    void clear_interrupt_flag(TimerNumber const tim_num) noexcept {
        *tim6_status_register = 0;
    }
}
