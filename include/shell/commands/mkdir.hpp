#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

class MkDirCommand final : public Command {
  public:
    MkDirCommand() = default;

    exit_code_t run() override;
};
