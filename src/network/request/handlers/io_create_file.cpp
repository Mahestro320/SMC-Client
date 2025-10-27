#include "network/request/handlers/io_create_file.hpp"

#include "network.hpp"
#include "network/client.hpp"

namespace fs = std::filesystem;
using boost::asio::ip::tcp;

bool IOCreateFileRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::IOCreateFile) || !network::sendString(socket, path.string())) {
        return false;
    }
    return network::checkResponse(socket);
}

void IOCreateFileRH::setFilePath(const fs::path& path) {
    this->path = path;
}
