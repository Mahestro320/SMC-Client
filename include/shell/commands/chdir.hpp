#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include "../command.hpp"

class ChDirCommand final : public Command {
  private:
    std::filesystem::path target_path_raw{};
    std::filesystem::path target_path_complete{};
    std::filesystem::path target_path_real{};

    bool getInputPath();
    void getRawInputPath();
    bool readCompleteInputPath();
    bool readRealInputPath();
    bool isInputPathDirectory() const;
    void setNewCurrentDir();

  public:
    ChDirCommand() = default;

    exit_code_t run() override;
};
