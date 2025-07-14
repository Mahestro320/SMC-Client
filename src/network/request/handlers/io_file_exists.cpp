#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_file_exists.hpp"

namespace fs = std::filesystem;
using boost::asio::ip::tcp;

bool IOFileExistsRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::IOFileExists)) {
        return false;
    }
    console::out::verbose("sending file path");
    if (!network::sendString(socket, path.generic_string())) {
        return false;
    }
    return processResponse(network::readResponse(socket));
}

bool IOFileExistsRH::processResponse(ResponseId id) {
    console::out::verbose("processing response");
    if (id == ResponseId::Ok) {
        does_file_exists = true;
    } else if (id == ResponseId::IOFileNotExists) {
        does_file_exists = false;
    } else {
        console::out::err("unknown response");
        return false;
    }
    return true;
}

void IOFileExistsRH::setPath(const fs::path& path) {
    this->path = path;
}

bool IOFileExistsRH::getValue() const {
    return does_file_exists;
}
