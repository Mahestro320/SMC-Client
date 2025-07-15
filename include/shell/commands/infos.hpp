#pragma once

#include "../command.hpp"

class InfosCommand : public Command {
  public:
    InfosCommand() = default;

    exit_code_t run() override;
};
