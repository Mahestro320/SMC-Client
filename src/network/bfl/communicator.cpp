#include "network/bfl/communicator.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/bfl/decoder.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;

BFLCommunicator::BFLCommunicator() {
    decoder = new BFLDecoder();
}

BFLCommunicator::~BFLCommunicator() {
    delete decoder;
}

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
    decoder->setBFL(bfl);
    return decoder->decode();
}

const std::vector<FileInfo>& BFLCommunicator::getData() const {
    return decoder->getData();
}
