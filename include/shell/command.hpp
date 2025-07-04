#pragma once

#include "network/client.hpp"

using exit_code_t = uint16_t;

enum ExitCode : exit_code_t {
    Success,
    Error,
};

class Command {
  protected:
    Client* client{};
    std::vector<std::string> args{};

  public:
    Command() = default;

    void setClient(Client* client);
    void setArgs(const std::vector<std::string>& args);

    virtual exit_code_t run() = 0;
};
