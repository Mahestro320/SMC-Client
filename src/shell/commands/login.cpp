#include "io/console.hpp"
#include "network/request/handlers/login.hpp"
#include "shell/commands/login.hpp"

using boost::asio::ip::tcp;

exit_code_t LoginCommand::run() {
    if (!getArgumentsValues()) {
        return Error;
    }
    LoginRH handler{};
    handler.setClient(client);
    handler.setUsername(username);
    handler.setPassword(password);
    if (!handler.run()) {
        return Error;
    }
    const User& user{handler.getFinalUser()};
    client->setUser(user);
    client->setLogged(true);
    console::out::verbose("-> ", false);
    console::out::inf("successfully connected as " + user.name + " (" + role::getName(user.role) + ")");
    return Success;
}

bool LoginCommand::getArgumentsValues() {
    if (args.size() < 1) {
        console::out::err("invalid arguments: missing username");
        return false;
    }
    username = args[0];
    if (args.size() >= 2) {
        password = args[1];
    }
    return checkUsername() && checkPassword();
}

bool LoginCommand::checkUsername() const {
    if (username.empty()) {
        console::out::err("empty username");
        return false;
    }
    if (username.size() > UINT8_MAX) {
        console::out::err("too long username");
        return false;
    }
    return true;
}

bool LoginCommand::checkPassword() const {
    if (password.size() > UINT8_MAX) {
        console::out::err("too long password");
        return false;
    }
    return true;
}
