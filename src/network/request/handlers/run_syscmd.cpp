#include "network/request/handlers/run_syscmd.hpp"

#include "network.hpp"
#include "network/client.hpp"

using boost::asio::ip::tcp;

bool RunSyscmdRH::run() {
    tcp::socket& socket{client->getSocket()};
    if (!network::sendRequest(socket, RequestId::RunSyscmd)) {
        return false;
    }
    return sendCommand(socket) && readOutput(socket);
}

bool RunSyscmdRH::sendCommand(tcp::socket& socket) const {
    return network::sendString(socket, command);
}

bool RunSyscmdRH::readOutput(tcp::socket& socket) {
    return network::readString(socket, output, true);
}

void RunSyscmdRH::setCommand(const std::string& command) {
    this->command = command;
}

const std::string& RunSyscmdRH::getOutput() const {
    return output;
}
