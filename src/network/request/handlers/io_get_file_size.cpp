#include "network/request/handlers/io_get_file_size.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

bool IOGetFileSizeRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::IOGetFileSize)) {
        return false;
    }
    return sendFilePath(socket) && network::checkResponse(socket) && getFileSize(socket);
}

bool IOGetFileSizeRH::sendFilePath(tcp::socket& socket) {
    console::out::verbose("sending file path");
    return network::sendString(socket, path.generic_string());
}

bool IOGetFileSizeRH::getFileSize(tcp::socket& socket) {
    console::out::verbose("getting file size");
    bool success{network::readInt<uint64_t>(socket, size)};
    console::out::verbose("file size: " + std::to_string(size));
    return success;
}

void IOGetFileSizeRH::setPath(const fs::path& path) {
    this->path = path;
}

uint64_t IOGetFileSizeRH::getValue() const {
    return size;
}
