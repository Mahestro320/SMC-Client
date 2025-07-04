#pragma once

#include "../command.hpp"

class IOFileExistsRH;

class CdCommand final : public Command {
  private:
    std::filesystem::path target_path_relative{}, target_path_absolute{};
    std::filesystem::path target_path_valid{};

    bool getTargetPath();
    bool fileExists();
    bool absolutePathExists(IOFileExistsRH& handler) const;
    bool relativePathExists(IOFileExistsRH& handler) const;
    bool isFileDirectory() const;
    void setNewUserCurrentDir();
    bool setUserCurrentDirRealPath();

  public:
    CdCommand() = default;

    exit_code_t run() override;
};
