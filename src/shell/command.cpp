#include "shell/command.hpp"

#include "network/client.hpp"

void Command::setClient(Client* client) {
    this->client = client;
}

void Command::setArgs(const std::vector<std::string>& args) {
    this->args = args;
}
