#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <string>
#include "../handler.hpp"

class RunSyscmdRH final : public RH {
  private:
    std::string command{}, output{};

    bool sendCommand(boost::asio::ip::tcp::socket& socket) const;
    bool readOutput(boost::asio::ip::tcp::socket& socket);

  public:
    RunSyscmdRH() = default;

    bool run() override;

    void setCommand(const std::string& command);
    const std::string& getOutput() const;
};
