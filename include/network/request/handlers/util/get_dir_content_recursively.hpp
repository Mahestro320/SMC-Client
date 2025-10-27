#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include <vector>
#include "../../handler.hpp"
#include "io/file_info.hpp"

class GetDirContentRecursively final : public RH {
  private:
    std::filesystem::path input_path{};

    std::vector<std::filesystem::path> content{};

    bool addContentOf(const std::filesystem::path& path);

  public:
    GetDirContentRecursively() = default;

    bool run() override;

    void setPath(const std::filesystem::path& path);
    const std::vector<std::filesystem::path>& getContent() const;
};
