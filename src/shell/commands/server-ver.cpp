#include "shell/commands/server-ver.hpp"

#include "io/console.hpp"
#include "network/request/handlers/get_version.hpp"

exit_code_t ServerVerCommand::run() {
    GetVersionRH handler{};
    handler.setClient(client);
    if (!handler.run()) {
        return Error;
    }
    console::out::inf("server version: " + handler.getValue());
    return Success;
}
