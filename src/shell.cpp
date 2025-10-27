#include "shell.hpp"

#include "constants.hpp"
#include "io/console.hpp"
#include "network/client.hpp"
#include "shell/command_line.hpp"
#include "shell/commands/chdir.hpp"
#include "shell/commands/clear.hpp"
#include "shell/commands/client.hpp"
#include "shell/commands/conf.hpp"
#include "shell/commands/delete.hpp"
#include "shell/commands/dir.hpp"
#include "shell/commands/download.hpp"
#include "shell/commands/help.hpp"
#include "shell/commands/infos.hpp"
#include "shell/commands/login.hpp"
#include "shell/commands/logout.hpp"
#include "shell/commands/mkdir.hpp"
#include "shell/commands/mkfile.hpp"
#include "shell/commands/print.hpp"
#include "shell/commands/quit.hpp"
#include "shell/commands/server-ver.hpp"
#include "shell/commands/syscmd.hpp"
#include "shell/commands/upload.hpp"
#include "shell/commands_infos.hpp"
#include "shell/config.hpp"
#include "shell/system/signal.hpp"
#include "util/string.hpp"
#include "util/time.hpp"

using boost::asio::ip::tcp;

Shell::Shell(Client& client) : client{client} {
}

void Shell::start() {
    shell::signal::setSignals(this);
    console::out::inf(constants::INFO_MESSAGE + ", type \"help\" for help");
}

void Shell::processNewCommand() {
    console::out::inf(getCommandInputStartInfo() + "> ", false);
    const std::vector<std::string> command_tokens{getCommandTokens()};
    if (command_tokens.empty()) {
        return;
    }
    const exit_code_t code{runCommand(command_tokens)};
    printFinalCmdMessage(code);
    console::out::inf();
    return;
}

std::vector<std::string> Shell::getCommandTokens() {
    CommandLine command_line{};
    if (!command_line.getLine() || command_line.empty()) {
        return {};
    }
    command_line.tokenize();
    return command_line.getTokens();
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
    uint64_t start_time{util::time::getMillis()};
    const exit_code_t code{command->run()};
    command_time = static_cast<float>(util::time::getMillis() - start_time) / 1000.0f;
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
    } else if (name == "print") {
        return new PrintCommand{};
    } else if (name == "login" || name == "lgi") {
        return new LoginCommand{};
    } else if (name == "logout" || name == "lgo") {
        return new LogoutCommand{};
    } else if (name == "client" || name == "clt") {
        return new ClientCommand{};
    } else if (name == "server-ver" || name == "srv-ver") {
        return new ServerVerCommand{};
    } else if (name == "dir") {
        return new DirCommand{};
    } else if (name == "chdir" || name == "cd") {
        return new ChDirCommand{};
    } else if (name == "download" || name == "dwl") {
        return new DownloadCommand{};
    } else if (name == "upload" || name == "upl") {
        return new UploadCommand{};
    } else if (name == "syscmd" || name == "scm") {
        return new SyscmdCommand{};
    } else if (name == "mkfile" || name == "mf") {
        return new MkFileCommand{};
    } else if (name == "mkdir" || name == "md") {
        return new MkDirCommand{};
    } else if (name == "delete" || name == "del") {
        return new DeleteCommand{};
    }
    return nullptr;
}

Client& Shell::getClient() {
    return client;
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
        return ((config_values.shell_print_addr_prefix) ? addr_prefix : "") + "NOT_LOGGED";
    }
    const User& user{client.getUser()};
    return ((config_values.shell_print_addr_prefix) ? addr_prefix : "") + user.name + " @" +
           ((user.current_dir.empty()) ? "." : user.current_dir.string());
}

void Shell::printFinalCmdMessage(exit_code_t code) const {
    if (code == ExitCode::Silent) {
        return;
    }
    if (code == ExitCode::InvalidArgs) {
        console::out::err("invalid command arguments");
        return;
    }
    std::string message_ext{};
    if (code == ExitCode::Error) {
        message_ext = "with exit code " + std::to_string(code) + " (Error)";
    } else if (code != ExitCode::Success) {
        message_ext = "with exit code " + std::to_string(code);
    }
    console::out::inf("\ncommand finished in " +
                      ((command_time < 1.0f) ? std::to_string(static_cast<int>(command_time * 1000.0f)) + "ms "
                                             : util::string::formatFloat(command_time, 3) + "s ") +
                      message_ext);
}
