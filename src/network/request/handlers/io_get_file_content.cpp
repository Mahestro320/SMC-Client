#include "io/console.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_file_content.hpp"
#include "network/request/handlers/io_get_file_size.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

bool IOGetFileContentRH::run() {
    socket = &client->getSocket();
    if (!getFileSize()) {
        return false;
    }
    calcBufferCount();
    return sendRequest() && sendBufferSize() && sendPath() && checkResponse();
}

bool IOGetFileContentRH::getFileSize() {
    IOGetFileSizeRH handler{};
    handler.setClient(client);
    handler.setPath(path);
    if (!handler.run()) {
        return false;
    }
    file_size = handler.getValue();
    return true;
}

void IOGetFileContentRH::calcBufferCount() {
    buffer_count = static_cast<uint64_t>(std::ceil(static_cast<double>(file_size) / buffer_size));
    console::out::verbose("buffer size: " + std::to_string(buffer_size));
    console::out::verbose("buffer count: " + std::to_string(buffer_count));
}

bool IOGetFileContentRH::sendRequest() const {
    return network::sendRequest(*socket, RequestId::IOGetFileContent);
}

bool IOGetFileContentRH::sendBufferSize() const {
    return network::sendInt<uint64_t>(*socket, buffer_size) && checkResponse();
}

bool IOGetFileContentRH::checkResponse() const {
    const ResponseId response_id{network::readResponse(*socket)};
    if (response_id != ResponseId::Ok) {
        console::out::err("server returned " + std::to_string(static_cast<uint8_t>(response_id)) + " (" +
                          network::response::getName(response_id) + ")");
        return false;
    }
    return true;
}

bool IOGetFileContentRH::sendPath() const {
    return network::sendString(*socket, path.generic_string()) && checkResponse();
}

void IOGetFileContentRH::setBufferSize(uint64_t size) {
    buffer_size = size;
}

void IOGetFileContentRH::setPath(const fs::path& path) {
    this->path = path;
}

bool IOGetFileContentRH::available() const {
    return curr_buffer_idx < buffer_count;
}

bool IOGetFileContentRH::downloadNextBuffer() {
    ++curr_buffer_idx;
    if (!network::readBuffer(*socket, curr_buffer, true) || !network::sendInt<uint8_t>(*socket, NEXT_BUFFER_FLAG)) {
        console::out::err("error while downloading buffer " + std::to_string(curr_buffer_idx));
        return false;
    }
    return true;
}

void IOGetFileContentRH::stop() {
    std::vector<char> unused_buffer{};
    network::readBuffer(*socket, unused_buffer, true) && network::sendInt<uint8_t>(*socket, STOP_FLAG);
}

uint64_t IOGetFileContentRH::getBufferCount() const {
    return buffer_count;
}

const std::vector<char>& IOGetFileContentRH::getCurrentBuffer() const {
    return curr_buffer;
}
