#pragma once

#include <boost/asio.hpp>
#include <concepts>
#include <filesystem>
#include "network/request/id.hpp"
#include "network/response/id.hpp"

class Client;

namespace network {

extern bool sendRequest(boost::asio::ip::tcp::socket& socket, RequestId id, bool silent = false);
extern bool sendString(boost::asio::ip::tcp::socket& socket, const std::string& str, bool silent = false);
extern bool sendBuffer(boost::asio::ip::tcp::socket& socket, const std::vector<char>& buffer, bool silent = false);
extern ResponseId readResponse(boost::asio::ip::tcp::socket& socket, bool silent = false);
extern bool readString(boost::asio::ip::tcp::socket& socket, std::string& str, bool silent = false);
extern bool readBuffer(boost::asio::ip::tcp::socket& socket, std::vector<char>& buffer, bool silent = false);

template<std::integral T> extern bool sendInt(boost::asio::ip::tcp::socket& socket, T byte);
template<std::integral T> extern bool readInt(boost::asio::ip::tcp::socket& socket, T& byte);

} // namespace network
