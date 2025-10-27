#include "network/request/handlers/io_get_file_content.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_file_size.hpp"
#include "network/request/handlers/util/transmition_flags.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

bool IOGetFileContentRH::run() {
    socket = &client->getSocket();
    if (!sendRequest() || !sendBufferSize() || !sendPath() || !network::checkResponse(*socket) || !getFileSize()) {
        return false;
    }
    calcBufferCount();
    return true;
}

bool IOGetFileContentRH::sendRequest() const {
    return network::sendRequest(*socket, RequestId::IOGetFileContent);
}

bool IOGetFileContentRH::sendBufferSize() const {
    return network::sendInt<uint64_t>(*socket, buffer_size) && network::checkResponse(*socket);
}

bool IOGetFileContentRH::sendPath() const {
    return network::sendString(*socket, path.generic_string()) && network::checkResponse(*socket);
}

void IOGetFileContentRH::setBufferSize(uint64_t size) {
    buffer_size = size;
}

void IOGetFileContentRH::setPath(const fs::path& path) {
    this->path = path;
}

void IOGetFileContentRH::calcBufferCount() {
    buffer_count = static_cast<uint64_t>(std::ceil(static_cast<double>(file_size) / buffer_size));
    console::out::verbose("buffer size: " + std::to_string(buffer_size));
    console::out::verbose("buffer count: " + std::to_string(buffer_count));
}

bool IOGetFileContentRH::getFileSize() {
    return network::readInt<uint64_t>(*socket, file_size);
}

bool IOGetFileContentRH::available() const {
    return curr_buffer_idx < buffer_count;
}

bool IOGetFileContentRH::downloadNextBuffer() {
    ++curr_buffer_idx;
    boost::system::error_code ec{};
    if (curr_buffer.size() != buffer_size) {
        curr_buffer.resize(static_cast<size_t>(buffer_size));
    }
    const size_t bytes_read{
        boost::asio::read(*socket, boost::asio::buffer(curr_buffer, static_cast<size_t>(buffer_size)), ec)};
    if (ec) {
        console::out::err("error while downloading buffer " + std::to_string(curr_buffer_idx) + ": " + ec.message());
        return false;
    }
    if (bytes_read == 0) {
        console::out::err("error: no bytes where readed");
        return false;
    }
    resizeBufferIfNeeded();
    if (!network::sendInt<uint8_t>(*socket, transmition_flags::NEXT_BUFFER)) {
        console::out::err("error while reading buffer flag " + std::to_string(curr_buffer_idx));
        return false;
    }
    return true;
}

void IOGetFileContentRH::resizeBufferIfNeeded() {
    if (curr_buffer_idx == buffer_count) {
        curr_buffer.resize(static_cast<size_t>(file_size - (buffer_count - 1) * buffer_size));
    }
}

void IOGetFileContentRH::stop() {
    if (available()) {
        std::vector<char> unused_buffer{};
        network::readBuffer(*socket, unused_buffer, true) &&
            network::sendInt<uint8_t>(*socket, transmition_flags::STOP);
    }
}

uint64_t IOGetFileContentRH::getBufferCount() const {
    return buffer_count;
}

const std::vector<char>& IOGetFileContentRH::getCurrentBuffer() const {
    return curr_buffer;
}
