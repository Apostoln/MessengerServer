#ifndef MESSENGERSERVER_CLIENT_HPP
#define MESSENGERSERVER_CLIENT_HPP

#include <iostream>
#include <memory>

#include <asio.hpp>

using namespace asio;

struct Client {

        constexpr static const unsigned short BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE] = {0};
        std::shared_ptr<ip::tcp::socket> socket;

        Client(ip::tcp::socket&& socket_)
            : socket(std::make_shared<ip::tcp::socket>(std::move(socket_)))
        {
            std::cout << "Client created: " << socket->remote_endpoint() << std::endl;
        }


};


#endif //MESSENGERSERVER_CLIENT_HPP
