#include "network/client.hpp"

#include "io/console.hpp"
#include "shell/config.hpp"

using boost::asio::ip::tcp;

Client::~Client() {
    socket.close();
}

void Client::resetState() {
    user = User{};
    is_logged = false;
    is_connected = false;
}

bool Client::connect() {
    resetState();
    is_connected = connectToServer() && checkWelcomeMessage();
    return is_connected;
}

bool Client::connectToServer() {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    console::out::inf("connecting to server at " + config_values.server_address + ":" +
                      std::to_string(config_values.server_port));
    boost::system::error_code ec{};
    const auto endpoints{resolver.resolve(config_values.server_address, std::to_string(config_values.server_port), ec)};
    if (!checkErr(ec)) {
        return false;
    }
    boost::asio::connect(socket, endpoints, ec);
    if (!checkErr(ec)) {
        return false;
    }
    console::out::inf("successfully connected to server !");
    return true;
}

bool Client::checkWelcomeMessage() {
    std::array<char, 3> buffer{};
    console::out::verbose("getting welcome message...", false);
    boost::system::error_code ec{};
    std::size_t length{socket.read_some(boost::asio::buffer(buffer), ec)};
    if (ec) {
        console::out::err(" error: " + ec.message());
        return false;
    }
    if (length != 3) {
        console::out::err(" error: invalid size");
        return false;
    }
    std::string welcome_message{buffer.data(), length};
    if (!isWelcomeMessage(welcome_message)) {
        console::out::err(" error: invalid message: " + welcome_message);
        return false;
    }
    console::out::verbose(" done !");
    return true;
}

bool Client::isWelcomeMessage(const std::string& message) {
    return message[0] == WELCOME_MESSAGE[0] && message[1] == WELCOME_MESSAGE[1] && message[2] == WELCOME_MESSAGE[2];
}

bool Client::checkErr(const boost::system::error_code& ec) {
    if (!ec) {
        return true;
    }
    console::out::err("error while connecting to server: " + ec.message());
    return false;
}

void Client::close() {
    resetState();
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

bool Client::isConnected() const {
    return is_connected;
}

void Client::setUser(const User& user) {
    this->user = user;
}

void Client::setLogged(bool is_logged) {
    this->is_logged = is_logged;
}
