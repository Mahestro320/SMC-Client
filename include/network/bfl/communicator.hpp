#pragma once

#include <vector>
#include "system/beg.hpp"

struct FileInfo;
class BFLDecoder;
class Client;

class BFLCommunicator {
  private:
    Client* client{};
    BFLDecoder* decoder{};

  public:
    BFLCommunicator();
    ~BFLCommunicator();

    void setClient(Client* client);
    bool run();
    const std::vector<FileInfo>& getData() const;
};
