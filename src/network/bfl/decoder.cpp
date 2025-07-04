#include "io/console.hpp"
#include "network/bfl/decoder.hpp"

void BFLDecoder::setBFL(const std::vector<char>& bfl) {
    this->bfl = bfl;
}

bool BFLDecoder::decode() {
    console::out::verbose("decoding BFL data... ", false);
    if (!getFileCount()) {
        return false;
    }
    for (uint64_t i{0}; i < file_count; ++i) {
        if (!decodeFile()) {
            return false;
        }
    }
    console::out::verbose("done");
    return true;
}

bool BFLDecoder::getFileCount() {
    if (bfl.size() < sizeof(uint64_t)) {
        console::out::err("can't get file count: invalid size");
        return false;
    }
    std::memcpy(&file_count, bfl.data() + current_offset, sizeof(uint64_t));
    current_offset += sizeof(uint64_t);
    return true;
}

bool BFLDecoder::decodeFile() {
    FileInfo file_info{};
    if (!getFileType(file_info.type) || !getFileSize(file_info.size) || !getFileName(file_info.name)) {
        return false;
    }
    data.push_back(file_info);
    return true;
}

bool BFLDecoder::getFileType(FileType& type) {
    if (bfl.size() < current_offset + sizeof(FileType)) {
        console::out::err("can't get file type: invalid size");
        return false;
    }
    std::memcpy(&type, bfl.data() + current_offset, sizeof(FileType));
    current_offset += sizeof(FileType);
    return true;
}

bool BFLDecoder::getFileSize(uint64_t& size) {
    if (bfl.size() < current_offset + sizeof(uint64_t)) {
        console::out::err("can't get file name size: invalid size");
        return false;
    }
    std::memcpy(&size, bfl.data() + current_offset, sizeof(uint64_t));
    current_offset += sizeof(uint64_t);
    return true;
}

bool BFLDecoder::getFileName(std::string& name) {
    if (bfl.size() < current_offset + sizeof(uint64_t)) {
        console::out::err("can't get file name size: invalid size");
        return false;
    }
    uint64_t name_size{};
    std::memcpy(&name_size, bfl.data() + current_offset, sizeof(uint64_t));
    current_offset += sizeof(uint64_t);
    if (bfl.size() < current_offset + name_size) {
        console::out::err("can't get file name: invalid size");
        return false;
    }
    name = std::string{bfl.data() + current_offset, name_size};
    current_offset += name_size;
    return true;
}

const std::vector<FileInfo>& BFLDecoder::getData() const {
    return data;
}
