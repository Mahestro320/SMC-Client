#include "io/console.hpp"
#include "network.hpp"
#include "network/request/handlers/get_user_role.hpp"

using boost::asio::ip::tcp;

bool GetUserRoleRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::GetUserRole)) {
        return false;
    }
    bool is_valid_role{getUserRole(socket)};
    bool is_valid_response{checkResponse(socket)};
    return is_valid_role && is_valid_response;
}

bool GetUserRoleRH::getUserRole(tcp::socket& socket) {
    console::out::verbose("getting user role... ", false);
    uint8_t byte{};
    if (!network::readInt(socket, byte)) {
        return false;
    }
    role = static_cast<Role>(byte);
    console::out::verbose(std::to_string(byte) + " (" + role::getName(role) + ")");
    return role::isValidRole(role);
}

bool GetUserRoleRH::checkResponse(tcp::socket& socket) const {
    ResponseId response{network::readResponse(socket)};
    if (response != ResponseId::Ok) {
        console::out::err("server returned " + std::to_string(static_cast<uint8_t>(response)) + " (" +
                          network::response::getName(response) + ")");
        return false;
    }
    return true;
}

Role GetUserRoleRH::getRole() const {
    return role;
}
