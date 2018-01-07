#ifndef MESSENGERSERVER_MESSENGERSERVER_HPP
#define MESSENGERSERVER_MESSENGERSERVER_HPP

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include <asio.hpp>

#include "Client.hpp"
#include "Registrar.hpp"

using namespace asio;

enum class ProtocolMessage;

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
        std::mutex clientsMutex;

        Registrar registrar;



    public:
        MessengerServer(size_t port);

        void run();

        void acceptClients();
        void handleClients();

        static bool isProtocolMessage(const char*);
        void handleProtocol(Client&, ProtocolMessage);
        void removeClosedClients();

};


#endif //MESSENGERSERVER_MESSENGERSERVER_HPP
