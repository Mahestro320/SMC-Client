#pragma once

#include "system/beg.hpp"

#include <string>

namespace util::byte {

template<typename T = uint64_t> std::string toAutoUnit(uint64_t num);

} // namespace util::byte

#include "byte.ipp"
