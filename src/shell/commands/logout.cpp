#include "network/client.hpp"
#include "network/request/handlers/logout.hpp"
#include "shell/commands/logout.hpp"

exit_code_t LogoutCommand::run() {
    LogoutRH handler{};
    handler.setClient(client);
    if (!handler.run()) {
        return Error;
    }
    client->setLogged(false);
    client->setUser({});
    return Success;
}
