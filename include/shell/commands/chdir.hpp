#pragma once

#include "../command.hpp"

class ChdirCommand final : public Command {
  private:
    std::filesystem::path target_path{};
    std::filesystem::path target_path_valid{};

    bool getTargetPath();
    bool getCompletePath();
    bool isFileDirectory() const;
    void setNewUserCurrentDir();
    bool setUserCurrentDirRealPath();

  public:
    ChdirCommand() = default;

    exit_code_t run() override;
};
