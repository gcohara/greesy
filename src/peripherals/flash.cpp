#include "../../inc/peripherals/flash.hpp"
#include <cstdint>

namespace {
    reg32 base{ (reg32) 0x40022000 };
    reg32 access_control_register{ base };
}

namespace Flash {
    void set_access_latency(AccessLatency const latency) noexcept {
        // clear
        *access_control_register &= (~0u << 3);
        // set
        *access_control_register |= static_cast<std::uint32_t>(latency);
    }
}
