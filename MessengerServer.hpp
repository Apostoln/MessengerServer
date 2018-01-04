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

        constexpr static const unsigned short BUFFER_SIZE = 1024;

    public:
        MessengerServer(size_t port)
            : serverEndPoint(ip::tcp::v4(), port),
              acceptor(ioService, serverEndPoint)
        {
            std::cout << "Server is created" << std::endl;
        }

        void run() {
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

};


#endif //MESSENGERSERVER_MESSENGERSERVER_HPP
