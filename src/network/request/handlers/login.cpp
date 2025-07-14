#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/get_user_role.hpp"
#include "network/request/handlers/login.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

bool LoginRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::Login)) {
        return false;
    }
    console::out::verbose("sending username and password");
    if (!sendUserInfos(socket)) {
        return false;
    }
    if (!checkResponse(socket) || !getUserRole()) {
        return false;
    }
    buildUser();
    return true;
}

bool LoginRH::sendUserInfos(tcp::socket& socket) const {
    return network::sendString(socket, final_user.name) && network::sendString(socket, final_user.password);
}

bool LoginRH::checkResponse(tcp::socket& socket) const {
    ResponseId response_id{network::readResponse(socket)};
    if (response_id != ResponseId::Ok) {
        console::out::err("server returned " + std::to_string(static_cast<uint8_t>(response_id)) + " (" +
                          network::response::getName(response_id) + ")");
        return false;
    }
    return true;
}

bool LoginRH::getUserRole() {
    GetUserRoleRH handler{};
    handler.setClient(client);
    if (!handler.run()) {
        return false;
    }
    final_user.role = handler.getValue();
    return true;
}

void LoginRH::buildUser() {
    if (final_user.role == Role::User) {
        final_user.current_dir = fs::path{"perso." + final_user.name};
    } else if (final_user.role == Role::Admin) {
        final_user.current_dir = fs::path{"network"} / ("perso." + final_user.name);
    } else if (final_user.role == Role::Developer) {
        final_user.current_dir = fs::path{"C:\\"};
    }
}

void LoginRH::setUsername(std::string username) {
    final_user.name = username;
}

void LoginRH::setPassword(std::string password) {
    final_user.password = password;
}

const User& LoginRH::getFinalUser() const {
    return final_user;
}
