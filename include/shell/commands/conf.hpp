#pragma once

#include "../command.hpp"

class ConfCommand final : public Command {
  private:
    bool runOption(const std::string& name);
    bool runLoadOption() const;
    bool runUpdateFileOption() const;
    bool runPrintOption() const;
    bool runSetOption() const;
    bool runGetOption() const;

  public:
    ConfCommand() = default;

    exit_code_t run() override;
};
