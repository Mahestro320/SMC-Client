#include "network/client.hpp"
#include "shell/command.hpp"

void Command::setClient(Client* client) {
    this->client = client;
}

void Command::setArgs(const std::vector<std::string>& args) {
    this->args = args;
}
