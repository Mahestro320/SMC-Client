#pragma once

#define _WIN32_WINNT 0x0601

#include <boost/asio.hpp>
#include "../handler.hpp"
#include "user/role.hpp"

class GetUserRoleRH final : public RH {
  private:
    Role role{Role::None};

    bool checkResponse(boost::asio::ip::tcp::socket& socket) const;
    bool getRole(boost::asio::ip::tcp::socket& socket);

  public:
    GetUserRoleRH() = default;

    bool run() override;
    Role getValue() const;
};
