#ifndef MESSENGERSERVER_MESSENGERSERVER_HPP
#define MESSENGERSERVER_MESSENGERSERVER_HPP

#include <iostream>
#include <vector>

#include <asio.hpp>

#include "Client.hpp"

using namespace asio;

class MessengerServer {
    private:
        io_service ioService;
        std::vector<Client> clients;
        ip::tcp::endpoint serverEndPoint;
        ip::tcp::acceptor acceptor;
        size_t port;
        //asio::ip::address interface;



    public:
        MessengerServer(size_t port);

        void run();
};


#endif //MESSENGERSERVER_MESSENGERSERVER_HPP
