#include "io/console.hpp"
#include "network.hpp"
#include "network/request/handlers/io_file_exists.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

namespace {

void pver(const std::string& msg, bool silent, bool line_return = true) {
    if (!silent) {
        console::out::verbose(msg, line_return);
    }
}

} // namespace

bool network::sendRequest(tcp::socket& socket, RequestId id, bool silent) {
    const uint8_t byte{static_cast<uint8_t>(id)};
    pver("sending request: " + std::to_string(byte) + " (" + request::getName(id) + ")", silent);
    if (!sendInt<uint8_t>(socket, byte)) {
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

bool network::sendString(tcp::socket& socket, const std::string& str, bool silent) {
    pver("sending string: \"" + str + "\"", silent);
    return sendBuffer(socket, std::vector<char>(str.begin(), str.end()));
}

bool network::sendBuffer(tcp::socket& socket, const std::vector<char>& buffer, bool silent) {
    const uint64_t buffer_size{buffer.size()};
    pver("sending buffer size (" + std::to_string(buffer_size) + ")", silent);
    if (!sendInt<uint64_t>(socket, buffer_size)) {
        return false;
    }
    pver("sending buffer content", silent);
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

bool network::readString(tcp::socket& socket, std::string& str, bool silent) {
    pver("reading string", silent);
    std::vector<char> data(str.begin(), str.end());
    if (!readBuffer(socket, data)) {
        return false;
    }
    str = std::string(data.begin(), data.end());
    pver("string content: \"" + str + "\"", silent);
    return true;
}

ResponseId network::readResponse(tcp::socket& socket, bool silent) {
    uint8_t byte{};
    pver("receiving response... ", silent, false);
    if (!readInt(socket, byte)) {
        return ResponseId::InvalidResponse;
    }
    ResponseId id{static_cast<ResponseId>(byte)};
    pver("done, id: " + std::to_string(byte) + " (" + response::getName(id) + ")", silent);
    return id;
}

bool network::readBuffer(tcp::socket& socket, std::vector<char>& buffer, bool silent) {
    pver("reading buffer size... ", silent, false);
    boost::system::error_code error_code{};
    size_t buffer_size{};
#if SIZE_MAX == UINT64_MAX
    if (!readInt<uint64_t>(socket, buffer_size)) {
        return false;
    }
#else
    uint64_t buffer_size_64{};
    if (!readInt<uint64_t>(socket, buffer_size_64)) {
        return false;
    }
    buffer_size = static_cast<size_t>(buffer_size_64);
#endif
    buffer.resize(buffer_size);
    pver("done (" + std::to_string(buffer_size) + ")\nreading buffer content... ", silent, false);
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(buffer, buffer_size), error_code)};
    if (error_code) {
        console::out::err("error: " + error_code.message());
        return false;
    }
    if (bytes_read < buffer_size) {
        console::out::err("the buffer isn't fully readed !");
        return false;
    }
    pver("done", silent);
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
