#include "network/request/handlers/io_create_directory.hpp"

#include "network.hpp"
#include "network/client.hpp"

namespace fs = std::filesystem;
using boost::asio::ip::tcp;

bool IOCreateDirectoryRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::IOCreateDirectory) || !network::sendString(socket, path.string())) {
        return false;
    }
    return network::checkResponse(socket);
}

void IOCreateDirectoryRH::setDirectoryPath(const fs::path& path) {
    this->path = path;
}
