#pragma once

#include "../handler.hpp"

struct FileInfo;

class GetDirContentRH : public RH {
  private:
    std::vector<FileInfo> data{};
    std::filesystem::path path{};

    bool sendPath(boost::asio::ip::tcp::socket& socket);

  public:
    GetDirContentRH() = default;

    bool run() override;

    const std::vector<FileInfo>& getData() const;
    void setPath(const std::filesystem::path& path);
};
