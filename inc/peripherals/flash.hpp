#ifndef FLASH_HPP
#define FLASH_HPP

#include "../main.hpp"

namespace Flash {
    enum class AccessLatency { Zero, One, Two };
    void set_access_latency(AccessLatency const latency) noexcept;
}

#endif
