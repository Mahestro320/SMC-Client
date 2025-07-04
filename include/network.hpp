#pragma once

#include <boost/asio.hpp>
#include <concepts>
#include "network/request/id.hpp"
#include "network/response/id.hpp"

namespace network {

extern bool sendRequest(boost::asio::ip::tcp::socket& socket, RequestId id);
extern bool sendString(boost::asio::ip::tcp::socket& socket, const std::string& str);
extern bool sendBuffer(boost::asio::ip::tcp::socket& socket, const std::vector<char>& buffer);
extern ResponseId readResponse(boost::asio::ip::tcp::socket& socket);
extern bool readString(boost::asio::ip::tcp::socket& socket, std::string& str);
extern bool readBuffer(boost::asio::ip::tcp::socket& socket, std::vector<char>& buffer);

template<std::integral T> extern bool sendInt(boost::asio::ip::tcp::socket& socket, T byte);
template<std::integral T> extern bool readInt(boost::asio::ip::tcp::socket& socket, T& byte);

} // namespace network
