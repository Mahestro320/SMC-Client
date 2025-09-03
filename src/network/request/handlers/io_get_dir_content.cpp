#include "network/request/handlers/io_get_dir_content.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/bfl/communicator.hpp"
#include "network/client.hpp"

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
    return network::checkResponse(socket);
}

const std::vector<FileInfo>& IOGetDirContentRH::getData() const {
    return data;
}

void IOGetDirContentRH::setPath(const std::filesystem::path& path) {
    this->path = path;
}
