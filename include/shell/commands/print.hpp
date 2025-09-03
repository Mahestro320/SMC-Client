#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

class PrintCommand final : public Command {
  public:
    PrintCommand() = default;

    exit_code_t run() override;
};
