#include <algorithm>
#include <memory>

#include "MessengerServer.hpp"


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

    char data[BUFFER_SIZE] = {0};
    while (true) {
        ip::tcp::socket socket(ioService);
        acceptor.accept(socket);
        size_t messageLength = socket.read_some(buffer(data));
        if (0 != messageLength) {
            std::cout << socket.remote_endpoint()
                      << " > " << data << std::endl;

            clients.push_back(std::make_shared<ip::tcp::socket>(std::move(socket)));

            for(auto& client: clients) {
                try {
                    client->write_some(buffer(data));
                    std::cout << client->remote_endpoint()
                              << " < " << data << std::endl;
                }
                catch(std::system_error &e) {
                    std::cerr << "Error: " << e.what() << std::endl
                              << "Code: " << e.code() << std::endl;
                    if(error::not_connected == e.code()) {
                        client.reset(); //delete connection and set pointer to null for furthering removing from vector
                    }
                }
            }

            clients.erase(std::remove(clients.begin(), clients.end(), nullptr), clients.end()); //removing nullptrs
        }
    }
}