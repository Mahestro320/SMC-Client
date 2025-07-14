#pragma once

#include "../command.hpp"

class ClearCommand final : public Command {
  public:
    ClearCommand() = default;

    exit_code_t run() override;
};
