#include <iostream>
#include <algorithm>
#include <memory>

#include "MessengerServer.hpp"
#include "ProtocolMessage.hpp"

MessengerServer::MessengerServer(size_t port)
    : serverEndPoint(ip::tcp::v4(), port),
      acceptor(ioService, serverEndPoint),
      port(port)
{
    std::cout << "Server is created" << std::endl;
}

void MessengerServer::run() {
    std::cout << "Server is started on " << serverEndPoint.address().to_string()
              << ":" << serverEndPoint.port() << std::endl;

    acceptorThread = std::thread(&MessengerServer::acceptClients, this);
    handlingThread = std::thread(&MessengerServer::handleClients, this);
    acceptorThread.join();
    handlingThread.join();
}

void MessengerServer::acceptClients() {
    while(true) {
        ip::tcp::socket socket(ioService);
        acceptor.accept(socket);

        Client client(std::move(socket));

        size_t messageLength = client.socket->read_some(buffer(client.buffer));
        if (0 != messageLength) {
            std::cout << client.endPoint
                      << " >> " << client.buffer << std::endl;
            if (ProtocolMessage::START == client.buffer) {
                client.write(ProtocolMessage::OK);

                std::lock_guard<std::mutex> clientsLock(clientsMutex);
                std::cout << "Client added to vector" << std::endl;
                clients.push_back(client);
            }
            else {
                std::cerr << "Request is not #OK" << std::endl;
                socket.shutdown(ip::tcp::socket::shutdown_send);
            }
        }
    }

}


void MessengerServer::handleClients() {
    while(true) {
        std::lock_guard<std::mutex> clientsLock(clientsMutex);
        for(auto& client: clients) {
            if (nullptr == client.socket) {
                continue;
            }
            size_t messageLength = client.socket->available() ?
                                   client.socket->read_some(buffer(client.buffer)) :
                                   0;

            if (0 != messageLength) {
                const char* message = client.buffer;
                std::cout << client.endPoint
                          << " > " << message << std::endl;

                if (isProtocolMessage(message)) {
                    handleProtocol(client, fromString(message));
                }
                else {
                    for (auto &outClient: clients) {
                        try {
                            outClient.write(message);
                        }
                        catch (std::system_error &e) {
                            std::cerr << "Error: " << e.what() << std::endl
                                      << "Code: " << e.code() << std::endl;
                            if (error::not_connected == e.code()) {
                                closeClient(outClient);
                                //delete connection and set pointer to null for furthering removing from vector
                            }
                        }
                    }
                }
            }
        }

        removeClosedClients();
    }
}

bool MessengerServer::isProtocolMessage(const char* msg) {
    if ( '#' == msg[0]) {
        return true;
    }
    return false;
}

void MessengerServer::handleProtocol(Client& client, ProtocolMessage msg) {
    switch(msg) {
        case ProtocolMessage::CANCEL: {
            client.write(ProtocolMessage::OK);
            closeClient(client);
            break;
        }
        default: {
            break;
        }
    }
}

void MessengerServer::closeClient(Client& client) {
    try { //TODO: cout for id
        std::cout << "Close client " << client.endPoint << std::endl;

    }
    catch (std::system_error &e){
        if (error::not_connected != e.code()) {
            throw; //else ignore
        }
    }
    client.socket.reset(); //delete connection and set pointer to null for furthering removing from vector
}

void MessengerServer::removeClosedClients() {
    clients.erase(std::remove_if(clients.begin(),
                                 clients.end(),
                                 [](auto client){
                                     return nullptr == client.socket;
                                 }),
                  clients.end()); //removing nullptrs
}