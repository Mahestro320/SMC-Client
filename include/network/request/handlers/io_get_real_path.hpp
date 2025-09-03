#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include "../handler.hpp"

class IOGetRealPathRH final : public RH {
  private:
    std::filesystem::path input_path{}, real_path{};

    bool sendInputPath(boost::asio::ip::tcp::socket& socket);
    bool getRealPath(boost::asio::ip::tcp::socket& socket);

  public:
    IOGetRealPathRH() = default;

    bool run() override;

    void setPath(const std::filesystem::path& input_path);
    const std::filesystem::path& getValue() const;
};
