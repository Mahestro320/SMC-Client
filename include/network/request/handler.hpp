#pragma once

#include "system/beg.hpp"

class Client;

// Request Handler
class RH {
  protected:
    Client* client{};

  public:
    RH() = default;

    void setClient(Client* client);
    [[nodiscard]] virtual bool run() = 0;
};
