#include "io/console.hpp"
#include "network.hpp"
#include "network/bfl/communicator.hpp"
#include "network/request/handlers/get_dir_content.hpp"

using boost::asio::ip::tcp;

bool GetDirContentRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::GetDirContent) || !sendPath(socket)) {
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

bool GetDirContentRH::sendPath(tcp::socket& socket) {
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

const std::vector<FileInfo>& GetDirContentRH::getData() const {
    return data;
}

void GetDirContentRH::setPath(const std::filesystem::path& path) {
    this->path = path;
}
