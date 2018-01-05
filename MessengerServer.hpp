#ifndef MESSENGERSERVER_MESSENGERSERVER_HPP
#define MESSENGERSERVER_MESSENGERSERVER_HPP

#include <iostream>
#include <vector>
#include <thread>

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
        //asio::ip::address interface; //TODO
        std::thread acceptorThread;
        std::thread handlingThread;



    public:
        MessengerServer(size_t port);

        void run();

        void acceptClients();
        void handleClients();

};


#endif //MESSENGERSERVER_MESSENGERSERVER_HPP
