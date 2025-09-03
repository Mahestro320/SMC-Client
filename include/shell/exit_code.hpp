#pragma once

#include "system/beg.hpp"

#include <stdint.h>

using exit_code_t = uint16_t;

enum ExitCode : exit_code_t {
    Success,
    Error,
    InvalidArgs,
    Silent,
};
