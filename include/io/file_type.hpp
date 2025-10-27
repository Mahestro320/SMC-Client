#pragma once

#include "system/beg.hpp"

#include <string>

enum class FileType : uint8_t {
    None,
    File,
    Directory,
};

namespace file_type {

extern std::string getName(FileType id);
extern bool isValid(FileType id);

} // namespace file_type
