#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

class LogoutCommand final : public Command {
  public:
    LogoutCommand() = default;

    exit_code_t run() override;
};
