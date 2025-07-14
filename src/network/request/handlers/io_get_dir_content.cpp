#include "io/console.hpp"
#include "network.hpp"
#include "network/bfl/communicator.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_dir_content.hpp"

using boost::asio::ip::tcp;

bool IOGetDirContentRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::IOGetDirContent) || !sendPath(socket)) {
        return false;
    }
    BFLCommunicator communicator{};
    communicator.setClient(client);
    if (!communicator.run()) {
        return false;
    }
    data = communicator.getData();
    return true;
}

bool IOGetDirContentRH::sendPath(tcp::socket& socket) {
    console::out::verbose("sending path");
    if (!network::sendString(socket, path.string())) {
        return false;
    }
    const ResponseId response_id{network::readResponse(socket)};
    if (response_id != ResponseId::Ok) {
        console::out::err("server returned " + std::to_string(static_cast<uint8_t>(response_id)) + " (" +
                          network::response::getName(response_id) + ")");
        return false;
    }
    return true;
}

const std::vector<FileInfo>& IOGetDirContentRH::getData() const {
    return data;
}

void IOGetDirContentRH::setPath(const std::filesystem::path& path) {
    this->path = path;
}
