#pragma once

#include "system/beg.hpp"

#include "../command.hpp"

class ServerVerCommand : public Command {
  public:
    ServerVerCommand() = default;

    exit_code_t run() override;
};
