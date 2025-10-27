#include "network/request/handlers/get_version.hpp"

#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;

bool GetVersionRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::GetVersion) || !network::readString(socket, version)) {
        return false;
    }
    return true;
}

const std::string& GetVersionRH::getValue() const {
    return version;
}
