#include "common.hpp"
#include "io/console.hpp"
#include "network/client.hpp"
#include "shell.hpp"
#include "shell/command_line.hpp"
#include "shell/commands/chdir.hpp"
#include "shell/commands/clear.hpp"
#include "shell/commands/client.hpp"
#include "shell/commands/conf.hpp"
#include "shell/commands/dir.hpp"
#include "shell/commands/download.hpp"
#include "shell/commands/help.hpp"
#include "shell/commands/infos.hpp"
#include "shell/commands/login.hpp"
#include "shell/commands/logout.hpp"
#include "shell/commands/quit.hpp"
#include "shell/commands_infos.hpp"
#include "shell/config.hpp"
#include "util/string.hpp"

using boost::asio::ip::tcp;

Shell::Shell(Client& client) : client{client} {
}

void Shell::start() {
    console::out::inf("SMC (Super Mega Cool) Client Shell v" + common::VERSION.toString() +
                      " by Mahestro_320\ntype \"help\" for help");
}

void Shell::processNewCommand() {
    console::out::inf(getCommandInputStartInfo() + "> ", false);
    CommandLine command{};
    if (!command.getLine()) {
        console::out::inf();
    }
    if (command.empty()) {
        return;
    }
    command.tokenize();
    const std::vector<std::string>& command_tokens{command.getTokens()};
    if (command_tokens.empty()) {
        return;
    }
    const exit_code_t code{runCommand(command_tokens)};
    if (code != Success) {
        console::out::inf("command finished with exit code " + std::to_string(code) +
                          (code == Error ? " (Error)" : ""));
    }
    console::out::inf();
    return;
}

exit_code_t Shell::runCommand(const std::vector<std::string>& command_tokens) {
    std::string name{command_tokens[0]};
    util::string::toLowercase(name);
    const std::vector<std::string> args{getArgsFromTokens(command_tokens)};
    Command* command{getCommandInstanceFromName(name)};
    if (!command) {
        console::out::err("unknown command: " + name);
        return Success;
    }
    if (!canExecuteCmdName(name)) {
        return Error;
    }
    command->setArgs(args);
    command->setClient(&client);
    const exit_code_t code{command->run()};
    delete command;
    return code;
}

bool Shell::canExecuteCmdName(const std::string& name) const {
    for (const CommandInfos& cmd : commands_infos) {
        if (cmd.name == name || cmd.short_name == name) {
            return canExecuteCmdInstance(cmd);
        }
    }
    console::out::err("no command found for " + name);
    return true;
}

bool Shell::canExecuteCmdInstance(const CommandInfos& cmd) const {
    const User& user{client.getUser()};
    if (!cmd.must_be_logged) {
        return true;
    }
    std::string error_message{};
    if (!client.isLogged()) {
        error_message = "not logged";
    } else if (user.role < cmd.min_role) {
        error_message = "invalid permissions";
    }
    if (!error_message.empty()) {
        console::out::err("can't run this command: " + error_message);
        return false;
    }
    return true;
}

Command* Shell::getCommandInstanceFromName(const std::string& name) const {
    if (name == "help") {
        return new HelpCommand{};
    } else if (name == "infos" || name == "inf") {
        return new InfosCommand{};
    } else if (name == "clear" || name == "clr") {
        return new ClearCommand{};
    } else if (name == "quit") {
        return new QuitCommand{};
    } else if (name == "conf") {
        return new ConfCommand{};
    } else if (name == "login" || name == "lgi") {
        return new LoginCommand{};
    } else if (name == "logout" || name == "lgo") {
        return new LogoutCommand{};
    } else if (name == "client" || name == "clt") {
        return new ClientCommand{};
    } else if (name == "dir") {
        return new DirCommand{};
    } else if (name == "chdir" || name == "cd") {
        return new ChdirCommand{};
    } else if (name == "download" || name == "dwl") {
        return new DownloadCommand{};
    }
    return nullptr;
}

std::vector<std::string> Shell::getArgsFromTokens(std::vector<std::string> tokens) const {
    tokens.erase(tokens.begin());
    return tokens;
}

std::string Shell::getCommandInputStartInfo() const {
    if (!client.isConnected()) {
        return "NOT_CONNECTED";
    }
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    const std::string addr_prefix{config_values.server_address + ":" + std::to_string(config_values.server_port) +
                                  " -> "};
    if (!client.isLogged()) {
        return (config_values.shell_print_addr_prefix ? addr_prefix : "") + "NOT_LOGGED";
    }
    const User& user{client.getUser()};
    return (config_values.shell_print_addr_prefix ? addr_prefix : "") + user.name + " @" +
           (user.current_dir.empty() ? "." : user.current_dir.string());
}
