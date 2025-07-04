#pragma once

#include "../command.hpp"

class QuitCommand final : public Command {
  public:
    QuitCommand() = default;

    exit_code_t run() override;
};
