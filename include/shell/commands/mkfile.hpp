#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

class MkFileCommand final : public Command {
  public:
    MkFileCommand() = default;

    exit_code_t run() override;
};
