#pragma once

#include "../command.hpp"

class ConfCommand final : public Command {
  private:
    exit_code_t runLoadOption() const;
    exit_code_t runUpdateFileOption() const;
    exit_code_t runPrintValsOption() const;
    exit_code_t runSetOption() const;
    exit_code_t runGetOption() const;

  public:
    ConfCommand() = default;

    exit_code_t run() override;
};
