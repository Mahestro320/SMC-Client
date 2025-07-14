#pragma once

#include "../command.hpp"

class LogoutCommand final : public Command {
  public:
    LogoutCommand() = default;

    exit_code_t run() override;
};
