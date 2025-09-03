#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

class QuitCommand final : public Command {
  public:
    QuitCommand() = default;

    exit_code_t run() override;
};
