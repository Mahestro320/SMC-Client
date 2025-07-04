#pragma once

#include <boost/asio.hpp>
#include "user.hpp"

class Client final {
  private:
    static inline constexpr char WELCOME_MESSAGE[3]{'S', 'M', 'C'};

    boost::asio::io_context io_context{};
    boost::asio::ip::tcp::resolver resolver{io_context};
    boost::asio::ip::tcp::socket socket{io_context};

    User user{};
    bool is_logged{};

    bool connect();
    bool checkWelcomeMessage();
    bool isWelcomeMessage(const std::string& message);

  public:
    Client() = default;

    bool begin();
    void closeSocket();

    boost::asio::ip::tcp::socket& getSocket();
    User& getUser();
    const User& getUser() const;
    bool isLogged() const;

    void setLogged(bool is_logged);
    void setUser(const User& user);
};
