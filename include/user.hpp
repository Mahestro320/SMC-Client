#pragma once

#include <filesystem>
#include <string>
#include "user/role.hpp"

struct User final {
    std::string name{}, password{};
    Role role{Role::None};
    std::filesystem::path current_dir{""};
};
