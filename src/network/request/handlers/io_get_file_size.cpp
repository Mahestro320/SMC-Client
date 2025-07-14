#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_file_size.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

bool IOGetFileSizeRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::IOGetFileSize)) {
        return false;
    }
    return sendFilePath(socket) && checkResponse(socket) && getFileSize(socket);
}

bool IOGetFileSizeRH::sendFilePath(tcp::socket& socket) {
    console::out::verbose("sending file path");
    return network::sendString(socket, path.generic_string());
}

bool IOGetFileSizeRH::checkResponse(tcp::socket& socket) {
    ResponseId response_id{network::readResponse(socket)};
    if (response_id != ResponseId::Ok) {
        console::out::err("server returned " + std::to_string(static_cast<uint8_t>(response_id)) + " (" +
                          network::response::getName(response_id) + ")");
        return false;
    }
    return true;
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
