#pragma once

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"
#include "io/file_type.hpp"

class IOGetFileTypeRH final : public RH {
  private:
    std::filesystem::path path{};
    FileType file_type{FileType::None};

    bool sendPath(boost::asio::ip::tcp::socket& socket) const;
    bool getFileType(boost::asio::ip::tcp::socket& socket);

  public:
    IOGetFileTypeRH() = default;

    bool run() override;
    void setPath(const std::filesystem::path& path);
    FileType getValue() const;
};
