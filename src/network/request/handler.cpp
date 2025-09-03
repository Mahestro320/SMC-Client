#include "network/request/handler.hpp"

#include "network/client.hpp"

void RH::setClient(Client* client) {
    this->client = client;
}
