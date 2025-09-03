#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include "../handler.hpp"

class IOCreateFileRH : public RH {
  private:
    std::filesystem::path path{};

  public:
    IOCreateFileRH() = default;

    bool run() override;

    void setFilePath(const std::filesystem::path& path);
};
