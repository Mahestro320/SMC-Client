#include "io/console.hpp"
#include "network/client.hpp"
#include "shell/commands/login.hpp"

using boost::asio::ip::tcp;

exit_code_t LoginCommand::run() {
    if (!getArgumentsValues()) {
        return Error;
    }
    return login() ? Success : Error;
}

bool LoginCommand::login() {
    if (!getConnectedUser()) {
        return false;
    }
    client->setUser(*connected_user);
    client->setLogged(true);
    console::out::verbose("-> ", false);
    console::out::inf("successfully connected as " + connected_user->name + " (" + role::getName(connected_user->role) +
                      ")");
    return true;
}

bool LoginCommand::getConnectedUser() {
    handler.setClient(client);
    handler.setUsername(username);
    handler.setPassword(password);
    if (!handler.run()) {
        return false;
    }
    connected_user = &handler.getConnectedUser();
    return true;
}

bool LoginCommand::getArgumentsValues() {
    if (args.empty()) {
        console::out::err("invalid arguments: missing username");
        return false;
    }
    if (args.size() < 2) {
        console::out::err("invalid arguments: missing password");
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
