#pragma once

#include <string>
#include <vector>
#include "shell/command.hpp"

class Client;
struct CommandInfos;

class Shell final {
  private:
    Client& client;

    std::vector<std::string> getArgsFromTokens(std::vector<std::string> tokens) const;
    bool canExecuteCmdName(const std::string& name) const;
    bool canExecuteCmdInstance(const CommandInfos& cmd) const;
    Command* getCommandInstanceFromName(const std::string& name) const;
    std::string getCommandInputStartInfo() const;

  public:
    Shell(Client& client);

    void start();
    void processNewCommand();
    Client& getClient();
    exit_code_t runCommand(const std::vector<std::string>& command_tokens);
};
