#pragma once

#include "network/client.hpp"

// Request Handler
class RH {
  protected:
    Client* client{};

  public:
    RH() = default;

    void setClient(Client* client);
    virtual bool run() = 0;
};
