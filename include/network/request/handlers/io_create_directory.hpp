#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include "../handler.hpp"

class IOCreateDirectoryRH final : public RH {
  private:
    std::filesystem::path path{};

  public:
    IOCreateDirectoryRH() = default;

    bool run() override;

    void setDirectoryPath(const std::filesystem::path& path);
};
