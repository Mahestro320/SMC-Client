#pragma once

#include "../command.hpp"

class ClientCommand final : public Command {
  private:
    exit_code_t runConnectOption();
    exit_code_t runDisconnectOption();

  public:
    ClientCommand() = default;

    exit_code_t run() override;
};
