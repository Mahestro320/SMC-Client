#pragma once

#include "system/beg.hpp"

#include "util/version.hpp"

namespace constants {

inline constexpr Version VERSION{0, 4, 0};
inline const std::string INFO_MESSAGE{"SMC (Super Mega Cool) Client Shell v" + VERSION.toString() + " by Mahestro_320"};

} // namespace constants
