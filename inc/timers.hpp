#ifndef TIMERS_HPP
#define TIMERS_HPP

#include <cstdint>

namespace BasicTimers {
    enum class TimerNumber { Tim6, Tim7 };
    enum class MasterModeSelection { Reset, Enable, Update };
    using OnePulse = bool;
    using PrescaleValue = std::uint16_t;
    using AutoreloadValue = std::uint16_t;

    struct Config {
        MasterModeSelection master_mode;
        OnePulse onepulse;
        PrescaleValue psc_val;
        AutoreloadValue autorel_val;
    };

    void enable_basic_timer(TimerNumber tim_num, Config const& cfg) noexcept;
    void change_timer_prescale(TimerNumber tim_num, PrescaleValue psc) noexcept;
    void change_timer_autoreload(TimerNumber tim_num, AutoreloadValue arv) noexcept;
    void reset_timer(TimerNumber tim_num) noexcept;
}

#endif
