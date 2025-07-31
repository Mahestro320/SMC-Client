#include "io/console.hpp"
#include "network/client.hpp"
#include "shell/commands/client.hpp"

exit_code_t ClientCommand::run() {
    if (args.empty()) {
        console::out::inf("invalid command arguments");
        return Error;
    }
    return runOption(args[0]) ? Success : Error;
}

bool ClientCommand::runOption(const std::string& option) {
    if (option == "connect") {
        return runConnectOption();
    } else if (option == "disconnect") {
        return runDisconnectOption();
    }
    console::out::err("unknown option: " + option);
    return false;
}

bool ClientCommand::runConnectOption() {
    return client->connect();
}

bool ClientCommand::runDisconnectOption() {
    client->close();
    return true;
}
