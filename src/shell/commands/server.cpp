#include "io/console.hpp"
#include "shell/commands/server.hpp"

exit_code_t ServerCommand::run() {
    if (args.empty()) {
        console::out::inf("invalid command arguments");
        return Error;
    }
    if (args[0] == "connect") {
        return runConnectOption();
    } else {
        console::out::err("unknown option: " + std::string(args[0]));
        return false;
    }
    return Success;
}

exit_code_t ServerCommand::runConnectOption() {
    if (!client->connect()) {
        console::out::err("error while connecting to server");
        return Error;
    }
    console::out::inf("successfully connected to server");
    return Success;
}
