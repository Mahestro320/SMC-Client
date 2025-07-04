#pragma once

#include <filesystem>

namespace dirs {

const auto ROOT{std::filesystem::current_path()};
const auto DATA{ROOT / "data"};

}; // namespace dirs
