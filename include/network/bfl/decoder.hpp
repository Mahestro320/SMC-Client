#pragma once

#include <vector>
#include "io/file_info.hpp"

// BFL = Buffered File List
class BFLDecoder {
  private:
    std::vector<char> bfl{};
    std::vector<FileInfo> data{};

    size_t file_count{};
    size_t current_offset{};

    bool getFileCount();
    bool decodeFile();
    bool getFileType(FileType& type);
    bool getFileSize(size_t& size);
    bool getFileName(std::string& name);

  public:
    BFLDecoder() = default;

    void setBFL(const std::vector<char>& bfl);
    bool decode();
    const std::vector<FileInfo>& getData() const;
};
