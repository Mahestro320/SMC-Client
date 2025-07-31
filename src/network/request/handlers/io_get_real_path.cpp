#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_real_path.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

bool IOGetRealPathRH::run() {
    tcp::socket& socket{client->getSocket()};
    return network::sendRequest(socket, RequestId::IOGetRealPath) && sendInputPath(socket) && getRealPath(socket);
}

bool IOGetRealPathRH::sendInputPath(boost::asio::ip::tcp::socket& socket) {
    console::out::verbose("sending input path");
    if (!network::sendString(socket, input_path.generic_string())) {
        return false;
    }
    ResponseId response_id{network::readResponse(socket)};
    if (response_id != ResponseId::Ok) {
        console::out::err("server returned " + std::to_string(static_cast<uint8_t>(response_id)) + " (" +
                          network::response::getName(response_id) + ")");
        return false;
    }
    return true;
}

bool IOGetRealPathRH::getRealPath(boost::asio::ip::tcp::socket& socket) {
    std::string real_path_str{};
    if (!network::readString(socket, real_path_str)) {
        return false;
    }
    const fs::path real_path_raw{real_path_str};
    real_path = real_path_raw.lexically_normal();
    return true;
}

void IOGetRealPathRH::setPath(const std::filesystem::path& input_path) {
    this->input_path = input_path;
}

const std::filesystem::path& IOGetRealPathRH::getValue() const {
    return real_path;
}
