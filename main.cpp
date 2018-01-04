#include <iostream>
#include <thread>

#include <asio.hpp>

using namespace asio;

int main() {
    constexpr static unsigned short PORT = 1234;
    constexpr static unsigned short BUFFER_SIZE = 1024;
    io_service ioService;
    ip::tcp::endpoint serverEndPoint(ip::tcp::v4(), PORT);
    ip::tcp::acceptor acceptor(ioService, serverEndPoint);

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

            socket.write_some(buffer(data));
            std::cout << socket.remote_endpoint()
                      << " < " << data << std::endl;
        }


    }


    return 0;
}