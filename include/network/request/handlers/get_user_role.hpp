#pragma once

#include <boost/asio.hpp>
#include "../handler.hpp"
#include "user/role.hpp"

class GetUserRoleRH final : public RH {
  private:
    Role role{Role::None};

    bool getRole(boost::asio::ip::tcp::socket& socket);
    bool checkResponse(boost::asio::ip::tcp::socket& socket) const;

  public:
    GetUserRoleRH() = default;

    bool run() override;
    Role getValue() const;
};
