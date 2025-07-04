#pragma once

#include <boost/asio.hpp>
#include "../handler.hpp"
#include "user.hpp"

class LoginRH final : public RH {
  private:
    User final_user{};

    bool sendUserInfos(boost::asio::ip::tcp::socket& socket) const;
    bool checkResponse(boost::asio::ip::tcp::socket& socket) const;
    bool getUserRole();
    void buildUser();

  public:
    LoginRH() = default;

    bool run() override;

    void setUsername(std::string username);
    void setPassword(std::string password);

    const User& getFinalUser() const;
};
