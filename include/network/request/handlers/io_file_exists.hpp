#pragma once

#include <filesystem>
#include "../handler.hpp"
#include "network/response/id.hpp"

class IOFileExistsRH final : public RH {
  private:
    std::filesystem::path path{};
    bool does_file_exists{};

    bool processResponse(ResponseId id);

  public:
    IOFileExistsRH() = default;

    bool run() override;
    void setPath(const std::filesystem::path& path);
    bool getValue() const;
};
