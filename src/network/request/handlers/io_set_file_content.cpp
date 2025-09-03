#include "network/request/handlers/io_set_file_content.hpp"

#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_create_file.hpp"
#include "network/request/handlers/io_file_exists.hpp"
#include "network/request/handlers/util/transmition_flags.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

bool IOSetFileContentRH::run() {
    socket = &client->getSocket();
    if (!createFileIfNeeded()) {
        return false;
    }
    if (curr_buffer.size() != buffer_size) {
        curr_buffer.resize(buffer_size);
    }
    calcBufferCount();
    return sendRequest() && sendFileSize() && sendBufferSize() && sendPath() && network::checkResponse(*socket);
}

bool IOSetFileContentRH::createFileIfNeeded() {
    IOFileExistsRH io_file_exists{};
    io_file_exists.setClient(client);
    io_file_exists.setPath(path);
    if (!io_file_exists.run()) {
        return false;
    }
    if (io_file_exists.getValue()) {
        return true;
    }
    IOCreateFileRH io_create_file_rh{};
    io_create_file_rh.setClient(client);
    io_create_file_rh.setFilePath(path);
    return io_create_file_rh.run();
}

bool IOSetFileContentRH::sendFileSize() {
    return network::sendInt<uint64_t>(*socket, file_size);
}

void IOSetFileContentRH::calcBufferCount() {
    buffer_count = static_cast<uint64_t>(std::ceil(static_cast<double>(file_size) / buffer_size));
    console::out::verbose("buffer size: " + std::to_string(buffer_size));
    console::out::verbose("buffer count: " + std::to_string(buffer_count));
}

bool IOSetFileContentRH::sendRequest() const {
    return network::sendRequest(*socket, RequestId::IOSetFileContent);
}

bool IOSetFileContentRH::sendBufferSize() const {
    return network::sendInt<uint64_t>(*socket, buffer_size) && network::checkResponse(*socket);
}

bool IOSetFileContentRH::sendPath() const {
    return network::sendString(*socket, path.generic_string()) && network::checkResponse(*socket);
}

void IOSetFileContentRH::setBufferSize(uint64_t size) {
    buffer_size = size;
}

void IOSetFileContentRH::setPath(const fs::path& path) {
    this->path = path;
}

void IOSetFileContentRH::setFileSize(uint64_t size) {
    file_size = size;
}

bool IOSetFileContentRH::available() const {
    return curr_buffer_idx < buffer_count;
}

bool IOSetFileContentRH::uploadNextBuffer() {
    ++curr_buffer_idx;
    if (curr_buffer.size() != buffer_size) {
        curr_buffer.resize(buffer_size);
    }
    boost::system::error_code ec{};
    boost::asio::write(*socket, boost::asio::buffer(curr_buffer, buffer_size), ec);
    if (ec) {
        console::out::err("error while sending buffer " + std::to_string(curr_buffer_idx) + ": " + ec.message());
        return false;
    }
    if (!network::sendInt<uint8_t>(*socket, transmition_flags::NEXT_BUFFER)) {
        console::out::err("error while sending buffer flag " + std::to_string(curr_buffer_idx));
        return false;
    }
    return true;
}

void IOSetFileContentRH::stop() {
    const std::vector<char> unused_buffer(buffer_size);
    boost::asio::write(*socket, boost::asio::buffer(unused_buffer, buffer_size)) &&
        network::sendInt<uint8_t>(*socket, transmition_flags::STOP);
}

uint64_t IOSetFileContentRH::getBufferCount() const {
    return buffer_count;
}

std::vector<char>& IOSetFileContentRH::getCurrentBuffer() {
    return curr_buffer;
}
