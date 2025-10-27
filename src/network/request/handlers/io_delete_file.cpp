#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_delete_file.hpp"

namespace fs = std::filesystem;
using boost::asio::ip::tcp;

bool IODeleteFileRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::IODeleteFile) || !network::sendString(socket, path.string())) {
        return false;
    }
    return network::checkResponse(socket);
}

void IODeleteFileRH::setFilePath(const std::filesystem::path& path) {
    this->path = path;
}
