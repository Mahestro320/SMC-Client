#include "io/console.hpp"
#include "network/client.hpp"
#include "shell/config.hpp"

using boost::asio::ip::tcp;

bool Client::begin() {
    return connect() && checkWelcomeMessage();
}

bool Client::connect() {
    try {
        const Config& config{Config::getInstance()};
        const ConfigValues& config_values{config.getValues()};
        console::out::inf("connecting to server at " + config_values.server_address + ":" +
                          std::to_string(config_values.server_port));
        const auto endpoints{resolver.resolve(config_values.server_address, std::to_string(config_values.server_port))};
        boost::asio::connect(socket, endpoints);
    } catch (const std::exception& e) {
        console::out::err("error while connecting to server: " + std::string{e.what()});
        return false;
    }
    console::out::inf("successfully connected to server !");
    return true;
}

bool Client::checkWelcomeMessage() {
    try {
        std::array<char, 3> buffer{};
        console::out::verbose("getting welcome message...", false);
        std::size_t length{socket.read_some(boost::asio::buffer(buffer))};
        if (length != 3) {
            console::out::err(" error: invalid size");
            return false;
        }
        std::string welcome_message{buffer.data(), length};
        if (!isWelcomeMessage(welcome_message)) {
            console::out::err(" error: invalid message: " + welcome_message);
            return false;
        }
    } catch (const std::exception& e) {
        console::out::err(" error: " + std::string{e.what()});
        return false;
    }
    console::out::verbose(" done !");
    return true;
}

bool Client::isWelcomeMessage(const std::string& message) {
    return message[0] == WELCOME_MESSAGE[0] && message[1] == WELCOME_MESSAGE[1] && message[2] == WELCOME_MESSAGE[2];
}

void Client::closeSocket() {
    socket.close();
}

tcp::socket& Client::getSocket() {
    return socket;
}

User& Client::getUser() {
    return user;
}

const User& Client::getUser() const {
    return user;
}

bool Client::isLogged() const {
    return is_logged;
}

void Client::setUser(const User& user) {
    this->user = user;
}

void Client::setLogged(bool is_logged) {
    this->is_logged = is_logged;
}
