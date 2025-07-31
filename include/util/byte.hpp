#pragma once

#include <string>

namespace util::byte {

template<typename T = uint64_t> std::string toAutoUnit(uint64_t num) {
    const T num_cast{static_cast<T>(num)};
    if (num < 0x400) {
        return std::to_string(num_cast) + " b";
    } else if (num < 0x100000) {
        return std::to_string(num_cast / 0x400) + " Kb";
    } else if (num < 0x40000000) {
        return std::to_string(num_cast / 0x100000) + " Mb";
    } else if (num < 0x10000000000) {
        return std::to_string(num_cast / 0x40000000) + " Gb";
    } else if (num < 0x4000000000000) {
        return std::to_string(num_cast / 0x10000000000) + " Tb";
    }
    return std::to_string(num_cast) + " b";
}

} // namespace util::byte
