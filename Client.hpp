#ifndef MESSENGERSERVER_CLIENT_HPP
#define MESSENGERSERVER_CLIENT_HPP

#include <iostream>
#include <memory>

#include <asio.hpp>
#include "ProtocolMessage.hpp"
#include "Account.hpp"

using namespace asio;

struct Client {

        constexpr static const unsigned short BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE] = {0};
        std::shared_ptr<ip::tcp::socket> socket;
        ip::tcp::endpoint endPoint;

        bool auth = false;
        Account* account = nullptr;

        Client(ip::tcp::socket&& socket_);

        void write(const std::string& message);

        void write(ProtocolMessage command);

        void close();

        bool read();

        void setAccount(Account* acc);
};


#endif //MESSENGERSERVER_CLIENT_HPP
