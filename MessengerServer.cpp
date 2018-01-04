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

            clients.push_back(std::move(socket));

            for(auto& client: clients) {
                client.write_some(buffer(data));
                std::cout << client.remote_endpoint()
                          << " < " << data << std::endl;
            }
        }
    }
}