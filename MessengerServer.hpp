#ifndef MESSENGERSERVER_MESSENGERSERVER_HPP
#define MESSENGERSERVER_MESSENGERSERVER_HPP

#include <iostream>
#include <vector>

#include <asio.hpp>

using namespace asio;

class MessengerServer {
    private:
        io_service ioService;
        std::vector<ip::tcp::socket> clients;
        ip::tcp::endpoint serverEndPoint;
        ip::tcp::acceptor acceptor;
        size_t port;
        //asio::ip::address interface;

        constexpr static const unsigned short BUFFER_SIZE = 1024;

    public:
        MessengerServer(size_t port);

        void run();

};


#endif //MESSENGERSERVER_MESSENGERSERVER_HPP
