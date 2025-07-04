#include "io/console.hpp"
#include "network.hpp"

using boost::asio::ip::tcp;

bool network::sendRequest(tcp::socket& socket, RequestId id) {
    const uint8_t byte{static_cast<uint8_t>(id)};
    console::out::verbose("sending request: " + std::to_string(byte) + " (" + request::getName(id) + ")");
    if (!sendInt(socket, byte)) {
        return false;
    }
    const ResponseId response_id{readResponse(socket)};
    if (response_id != ResponseId::Ok) {
        console::out::err("server returned " + std::to_string(static_cast<uint8_t>(response_id)) + " (" +
                          response::getName(response_id) + ")");
        return false;
    }
    return true;
}

bool network::sendString(tcp::socket& socket, const std::string& str) {
    console::out::verbose("sending string: \"" + str + "\"");
    return sendBuffer(socket, std::vector<char>(str.begin(), str.end()));
}

bool network::sendBuffer(tcp::socket& socket, const std::vector<char>& buffer) {
    const uint64_t buffer_size{buffer.size()};
    console::out::verbose("sending buffer size (" + std::to_string(buffer_size) + ")");
    if (!sendInt(socket, buffer_size)) {
        return false;
    }
    console::out::verbose("sending buffer content");
    boost::system::error_code error_code{};
    uint64_t bytes_write{boost::asio::write(socket, boost::asio::buffer(buffer), error_code)};
    if (error_code) {
        console::out::err("error while sending buffer content: " + error_code.message());
        return false;
    }
    if (bytes_write < buffer_size) {
        console::out::err("the buffer isn't fully sent !");
        return false;
    }
    return true;
}

bool network::readString(tcp::socket& socket, std::string& str) {
    console::out::verbose("reading string");
    std::vector<char> data(str.begin(), str.end());
    if (!readBuffer(socket, data)) {
        return false;
    }
    str = std::string(data.begin(), data.end());
    console::out::verbose("string content: \"" + str + "\"");
    return true;
}

ResponseId network::readResponse(tcp::socket& socket) {
    uint8_t byte{};
    console::out::verbose("receiving response... ", false);
    if (!readInt(socket, byte)) {
        return ResponseId::InvalidResponse;
    }
    ResponseId id{static_cast<ResponseId>(byte)};
    console::out::verbose("done, id: " + std::to_string(byte) + " (" + response::getName(id) + ")");
    return id;
}

bool network::readBuffer(tcp::socket& socket, std::vector<char>& buffer) {
    console::out::verbose("reading buffer size... ", false);
    boost::system::error_code error_code{};
    uint64_t buffer_size{};
    if (!readInt(socket, buffer_size)) {
        return false;
    }
    buffer.resize(buffer_size);
    console::out::verbose("done (" + std::to_string(buffer_size) + ")\nreading buffer content... ", false);
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(buffer, buffer_size), error_code)};
    if (error_code) {
        console::out::err("error: " + error_code.message());
        return false;
    }
    if (bytes_read < buffer_size) {
        console::out::err("the buffer isn't fully readed !");
        return false;
    }
    console::out::verbose("done");
    return true;
}

template<std::integral T> bool network::sendInt(tcp::socket& socket, T data) {
    boost::system::error_code error_code{};
    boost::asio::write(socket, boost::asio::buffer(&data, sizeof(T)), error_code);
    if (error_code) {
        console::out::err("error while sending int: " + error_code.message());
        return false;
    }
    return true;
}

template<std::integral T> bool network::readInt(tcp::socket& socket, T& data) {
    boost::system::error_code error_code{};
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(&data, sizeof(T)), error_code)};
    if (error_code) {
        console::out::err("error while reading int: " + error_code.message());
        return false;
    }
    if (bytes_read < sizeof(T)) {
        console::out::err("the int isn't fully readed !");
        return false;
    }
    return true;
}
