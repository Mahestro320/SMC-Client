#include "io/console.hpp"
#include "network.hpp"
#include "network/request/handlers/io_get_file_type.hpp"

namespace fs = std::filesystem;
using boost::asio::ip::tcp;

bool IOGetFileTypeRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::IOGetFileType)) {
        return false;
    }
    return sendPath(socket) && getFileType(socket);
}

bool IOGetFileTypeRH::sendPath(tcp::socket& socket) const {
    console::out::verbose("sending file path");
    if (!network::sendString(socket, path.generic_string())) {
        return false;
    }
    return true;
}

bool IOGetFileTypeRH::getFileType(tcp::socket& socket) {
    uint8_t file_type_num{};
    if (!network::readInt(socket, file_type_num)) {
        return false;
    }
    FileType file_type_id{static_cast<FileType>(file_type_num)};
    if (!file_type::isValid(file_type_id)) {
        console::out::err("invalid file type");
        return false;
    }
    this->file_type = file_type_id;
    return true;
}

void IOGetFileTypeRH::setPath(const fs::path& path) {
    this->path = path;
}

FileType IOGetFileTypeRH::fileType() const {
    return file_type;
}
