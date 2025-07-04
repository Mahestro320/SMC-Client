#include "io/console.hpp"
#include "network.hpp"
#include "network/bfl/communicator.hpp"

using boost::asio::ip::tcp;

void BFLCommunicator::setClient(Client* client) {
    this->client = client;
}

bool BFLCommunicator::run() {
    tcp::socket& socket{client->getSocket()};
    std::vector<char> bfl{};
    console::out::verbose("reading BFL data");
    if (!network::readBuffer(socket, bfl)) {
        return false;
    }
    decoder.setBFL(bfl);
    return decoder.decode();
}

const std::vector<FileInfo>& BFLCommunicator::getData() const {
    return decoder.getData();
}
