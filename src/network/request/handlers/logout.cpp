#include "network/request/handlers/logout.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;

bool LogoutRH::run() {
    tcp::socket& socket{client->getSocket()};
    console::out::verbose("logging out");
    return network::sendRequest(socket, RequestId::Logout) && network::checkResponse(socket);
}
