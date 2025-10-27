#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include "../handler.hpp"

class IODeleteFileRH : public RH {
  private:
    std::filesystem::path path{};

  public:
    IODeleteFileRH() = default;

    bool run() override;

    void setFilePath(const std::filesystem::path& path);
};
