#pragma once

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"

class IOGetFileContentRH final : public RH {
  private:
    static inline constexpr uint8_t NEXT_BUFFER_FLAG{0xAB};

    uint64_t buffer_size{}, buffer_count{};
    uint64_t file_size{};
    std::filesystem::path path{};
    boost::asio::ip::tcp::socket* socket{};

    std::vector<char> curr_buffer{};
    uint64_t curr_buffer_idx{};

    bool getFileSize();
    void calcBufferCount();
    bool sendRequest() const;
    bool sendBufferSize() const;
    bool checkResponse() const;
    bool sendPath() const;

  public:
    IOGetFileContentRH() = default;

    bool run() override;
    bool available() const;
    bool downloadNextBuffer();

    void setBufferSize(uint64_t size);
    void setPath(const std::filesystem::path& path);

    uint64_t getBufferCount() const;
    const std::vector<char>& getCurrentBuffer() const;
};
