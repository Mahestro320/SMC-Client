#pragma once

#include "../client.hpp"
#include "decoder.hpp"

class BFLCommunicator {
  private:
    Client* client{};
    BFLDecoder decoder{};

  public:
    BFLCommunicator() = default;

    void setClient(Client* client);
    bool run();
    const std::vector<FileInfo>& getData() const;
};
