#pragma once

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"

class IOGetFileSizeRH final : public RH {
  private:
    uint64_t size{};
    std::filesystem::path path{};

    bool sendFilePath(boost::asio::ip::tcp::socket& socket);
    bool checkResponse(boost::asio::ip::tcp::socket& socket);
    bool getFileSize(boost::asio::ip::tcp::socket& socket);

  public:
    IOGetFileSizeRH() = default;

    bool run() override;

    void setPath(const std::filesystem::path& path);
    uint64_t getValue() const;
};
