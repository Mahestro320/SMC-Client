#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include "user/role.hpp"

struct User final {
    std::string name{}, password{};
    Role role{Role::None};
    std::filesystem::path current_dir{""};
};
