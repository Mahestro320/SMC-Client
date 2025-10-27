#pragma once

#include "system/beg.hpp"

#include <string>
#include "../command.hpp"

class SyscmdCommand : public Command {
  public:
    exit_code_t run() override;
};
