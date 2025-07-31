#pragma once

#include "../command.hpp"

class ChdirCommand final : public Command {
  private:
    std::filesystem::path target_path_raw{};
    std::filesystem::path target_path_complete{};
    std::filesystem::path target_path_real{};

    bool getInputPath();
    bool getRawInputPath();
    bool readCompleteInputPath();
    bool readRealInputPath();
    bool isInputPathDirectory() const;
    void setNewCurrentDir();

  public:
    ChdirCommand() = default;

    exit_code_t run() override;
};
