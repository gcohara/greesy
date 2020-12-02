#ifndef TIMERS_HPP
#define TIMERS_HPP

#include <cstdint>

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

    void enable_basic_timer(TimerNumber const tim_num, Config const& cfg) noexcept;
    // void change_timer_prescale(TimerNumber const tim_num, PrescaleValue psc) noexcept;
    // void change_timer_autoreload(TimerNumber const tim_num, AutoreloadValue arv) noexcept;
    // void reset_timer(TimerNumber const tim_num) noexcept;
    void clear_interrupt_flag(TimerNumber const tim_num) noexcept;
}

#endif
