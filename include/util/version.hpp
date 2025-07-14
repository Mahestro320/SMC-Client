#pragma once

#include <string>

struct Version final {
    uint16_t MAJOR{}, MINOR{}, PATCH{};

    std::string toString(char delim = '.') const;
};
