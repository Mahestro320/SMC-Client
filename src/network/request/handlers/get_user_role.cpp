#include "network/request/handlers/get_user_role.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;

bool GetUserRoleRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::GetUserRole)) {
        return false;
    }
    return network::checkResponse(socket) && getRole(socket);
}

bool GetUserRoleRH::getRole(tcp::socket& socket) {
    console::out::verbose("getting user role... ", false);
    uint8_t byte{};
    if (!network::readInt(socket, byte)) {
        return false;
    }
    role = static_cast<Role>(byte);
    console::out::verbose(std::to_string(byte) + " (" + role::getName(role) + ")");
    return role::isValidRole(role);
}

Role GetUserRoleRH::getValue() const {
    return role;
}
