#pragma once

#include "../command.hpp"

class ClientCommand final : public Command {
  private:
    bool runOption(const std::string& option);

    bool runConnectOption();
    bool runDisconnectOption();

  public:
    ClientCommand() = default;

    exit_code_t run() override;
};
