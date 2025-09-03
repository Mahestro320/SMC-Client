#pragma once

#include "system/beg.hpp"

#include <stdint.h>

namespace transmition_flags {

static inline constexpr uint8_t NEXT_BUFFER{0x00};
static inline constexpr uint8_t STOP{0xFF};

} // namespace transmition_flags
