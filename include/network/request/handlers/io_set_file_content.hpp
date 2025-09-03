#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"

class IOSetFileContentRH final : public RH {
  private:
    uint64_t buffer_size{}, buffer_count{};
    uint64_t file_size{};
    std::filesystem::path path{};
    boost::asio::ip::tcp::socket* socket{};

    std::vector<char> curr_buffer{};
    uint64_t curr_buffer_idx{};

    bool createFileIfNeeded();
    bool sendFileSize();
    void calcBufferCount();
    bool sendRequest() const;
    bool sendBufferSize() const;
    bool sendPath() const;

  public:
    IOSetFileContentRH() = default;

    bool run() override;
    bool available() const;
    bool uploadNextBuffer();
    void stop();

    void setBufferSize(uint64_t size);
    void setPath(const std::filesystem::path& path);
    void setFileSize(uint64_t size);

    uint64_t getBufferCount() const;
    std::vector<char>& getCurrentBuffer();
};
