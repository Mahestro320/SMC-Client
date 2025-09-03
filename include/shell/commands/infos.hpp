#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

class InfosCommand : public Command {
  public:
    InfosCommand() = default;

    exit_code_t run() override;
};
