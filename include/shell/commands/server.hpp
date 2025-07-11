#pragma once

#include "../command.hpp"

class ServerCommand final : public Command {
  private:
    exit_code_t runConnectOption();

  public:
    ServerCommand() = default;

    exit_code_t run() override;
};
