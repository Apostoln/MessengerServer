#include <algorithm>
#include <memory>

#include "MessengerServer.hpp"
#include "Client.hpp"



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
    constexpr static const unsigned short BUFFER_SIZE = 1024;
    //char data[BUFFER_SIZE] = {0};

    while (true) {
        ip::tcp::socket socket(ioService);
        acceptor.accept(socket);

        Client client(std::move(socket));

        size_t messageLength = client.socket->read_some(buffer(client.buffer));
        if (0 != messageLength) {
            std::cout << client.socket->remote_endpoint()
                      << " > " << client.buffer << std::endl;

            clients.push_back(client);

            for(auto& client: clients) {
                try {
                    client.socket->write_some(buffer(client.buffer));
                    std::cout << client.socket->remote_endpoint()
                              << " < " << client.buffer << std::endl;
                }
                catch(std::system_error &e) {
                    std::cerr << "Error: " << e.what() << std::endl
                              << "Code: " << e.code() << std::endl;
                    if(error::not_connected == e.code()) {
                        client.socket.reset(); //delete connection and set pointer to null for furthering removing from vector
                    }
                }
            }

            clients.erase(std::remove_if(clients.begin(),
                                      clients.end(),
                                      [](auto client){
                                          return nullptr == client.socket;
                                      }),
                          clients.end()); //removing nullptrs
        }
    }
}