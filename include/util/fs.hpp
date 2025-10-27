#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

namespace util::fs {

extern std::filesystem::path getNonExistingPath(const std::filesystem::path& path);
extern void writeBufferInFile(std::ofstream& file, const std::vector<char>& buffer);
extern bool pushDirContentIn(std::vector<std::filesystem::path>& vec, const std::filesystem::path& path);

template<typename StreamType> extern void freeStream(StreamType*& file);

} // namespace util::fs

#include "fs.ipp"
