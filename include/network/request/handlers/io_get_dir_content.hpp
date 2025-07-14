#pragma once

#include <boost/asio.hpp>
#include <filesystem>
#include <vector>
#include "../handler.hpp"
#include "io/file_info.hpp"

class IOGetDirContentRH final : public RH {
  private:
    std::vector<FileInfo> data{};
    std::filesystem::path path{};

    bool sendPath(boost::asio::ip::tcp::socket& socket);

  public:
    IOGetDirContentRH() = default;

    bool run() override;

    const std::vector<FileInfo>& getData() const;
    void setPath(const std::filesystem::path& path);
};
