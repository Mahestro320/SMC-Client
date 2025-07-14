#pragma once

#include "network/request/handlers/io_file_exists.hpp"

class IOGetCompletePathRH final : public RH {
  private:
    std::filesystem::path input_path_relative{}, input_path_absolute{};
    std::filesystem::path output_path{};
    bool does_file_exists{};
    bool error{};
    IOFileExistsRH handler{};

    bool isPathRelative();
    bool isPathAbsolute();

  public:
    IOGetCompletePathRH() = default;

    bool run() override;

    void setPath(const std::filesystem::path& path);
    bool fileExists() const;
    const std::filesystem::path& getValue() const;
};
