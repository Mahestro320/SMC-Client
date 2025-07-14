#include "io/console.hpp"
#include "network/client.hpp"
#include "shell/commands/client.hpp"

exit_code_t ClientCommand::run() {
    if (args.empty()) {
        console::out::inf("invalid command arguments");
        return Error;
    }
    if (args[0] == "connect") {
        return runConnectOption();
    } else if (args[0] == "disconnect") {
        return runDisconnectOption();
    } else {
        console::out::err("unknown option: " + std::string(args[0]));
        return false;
    }
    return Success;
}

exit_code_t ClientCommand::runConnectOption() {
    if (!client->connect()) {
        return Error;
    }
    console::out::inf("successfully connected to server");
    return Success;
}

exit_code_t ClientCommand::runDisconnectOption() {
    client->close();
    return Success;
}
