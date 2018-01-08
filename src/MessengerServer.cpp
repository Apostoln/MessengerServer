#include <iostream>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <sstream>

#include "easylogging++.h"

#include "MessengerServer.hpp"
#include "ProtocolMessage.hpp"


MessengerServer::MessengerServer(Registrar* registrar, size_t port)
    : serverEndPoint(ip::tcp::v4(), port),
      acceptor(ioService, serverEndPoint),
      port(port),
      registrar(registrar)
{
    LOG(DEBUG) << "Server is created";

}

void MessengerServer::run() {
    std::cout << "Server is started on " << serverEndPoint.address().to_string()
              << ":" << serverEndPoint.port() << std::endl;
    LOG(INFO) << "Server is started on " << serverEndPoint.address().to_string()
              << ":" << serverEndPoint.port();

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

        while(true) {
            if (client.read()) {
                if (ProtocolMessage::START == client.buffer) {
                    client.write(ProtocolMessage::OK);

                    std::lock_guard<std::mutex> clientsLock(clientsMutex);
                    LOG(DEBUG) << "Client added to vector";
                    clients.push_back(client);
                } else {
                    LOG(WARNING) << "Request is not #OK";
                    socket.shutdown(ip::tcp::socket::shutdown_send);
                }
                break;
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
            if (client.read()) {
                const char* message = client.buffer;

                if (isProtocolMessage(message)) {
                    handleProtocol(client, fromString(message));
                }
                else {
                    if (!client.auth) {
                        unathorized(client);
                        break;
                    }
                    for (auto &outClient: clients) {
                        if (!outClient.auth) {
                            continue; //pass unauth clients
                        }
                        if (outClient.socket == client.socket) {
                            continue; //don't send to sender
                        }
                        try {
                            outClient.write(wrapMessage(client, message));
                        }
                        catch (std::system_error &e) {
                            LOG(WARNING) << "Error: " << e.what() << std::endl
                                      << "Code: " << e.code();
                            if (error::not_connected == e.code()) {
                                outClient.close();
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
            client.close();
            break;
        }
        case ProtocolMessage::REG: {
            client.write(ProtocolMessage::OK);
            while(true) {
                if(client.read()) {
                    std::string message(client.buffer);
                    std::stringstream stream(message);
                    std::string temp;
                    std::getline(stream, temp, ' '); //split str for whitespace
                    std::string login = temp;
                    std::getline(stream, temp, ' ');
                    std::string password = temp;

                    registrar->addAccount(Account{login, password});
                    break;
                }
            }
            client.write(ProtocolMessage::OK);
            break;
        }
        case ProtocolMessage::LOGIN: {
            client.write(ProtocolMessage::OK);
            bool isAuth = false;
            while(true) {
                if(client.read()) {
                    std::string message(client.buffer);
                    std::stringstream stream(message);
                    std::string temp;
                    std::getline(stream, temp, ' '); //split str for whitespace
                    std::string login = temp;
                    std::getline(stream, temp, ' ');
                    std::string password = temp;

                    Account candidate{login, password};

                    auto result = registrar->authAccount(candidate);

                    if (result.first) { // if successfull
                        isAuth = true;
                        client.setAccount(result.second);
                    }

                    break;
                }
            }
            if (isAuth) {
                client.write(ProtocolMessage::OK);
            }
            else {
                client.write(ProtocolMessage::ERROR);
            }
        }
        default: {
            break;
        }
    }
}

void MessengerServer::removeClosedClients() {
    clients.erase(std::remove_if(clients.begin(),
                                 clients.end(),
                                 [](auto client){
                                     return nullptr == client.socket;
                                 }),
                  clients.end()); //removing nullptrs
}

void MessengerServer::unathorized(Client& client) {
    client.write(ProtocolMessage::UNAUTH);
}

std::string MessengerServer::wrapMessage(Client client, const std::string &message) {
    std::time_t t = std::time(nullptr);
    std::tm* tt = std::localtime(&t);
    std::stringstream buffer;
    buffer << std::put_time(tt, "%d.%m.%y %T");
    auto timeStamp = buffer.str();

    return timeStamp + " [" + client.account->login + "] " + message;
}