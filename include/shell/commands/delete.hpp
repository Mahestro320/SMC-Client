#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

class DeleteCommand final : public Command {
  public:
    DeleteCommand() = default;

    exit_code_t run() override;
};
