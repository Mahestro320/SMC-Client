#pragma once

#include "../command.hpp"

class ClsCommand final : public Command {
  public:
    ClsCommand() = default;

    exit_code_t run() override;
};
