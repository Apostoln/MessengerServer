#include "Client.hpp"
#include "MessengerServer.hpp"

Client::Client(ip::tcp::socket&& socket_)
    : socket(std::make_shared<ip::tcp::socket>(std::move(socket_))),
      endPoint(socket->remote_endpoint())
{
    std::cout << "Client created: " << endPoint << std::endl;
}

void Client::write(const std::string& message) {
    auto msgBuffer = asio::buffer(message);
    socket->write_some(msgBuffer);
    std::cout << endPoint
              << " < " << message << std::endl;
}

void Client::write(ProtocolMessage command) {
    std::string message = protocolString[command];
    auto msgBuffer = asio::buffer(message);
    socket->write_some(msgBuffer);
    std::cout << endPoint
              << " << " << message << std::endl;
}

void Client::close() {
    std::cout << "Close client " << endPoint << std::endl;
    socket.reset(); //delete connection and set pointer to null for furthering removing from vector
}

bool Client::read() {
    memset(&buffer[0], 0, sizeof(buffer));
    auto delimiter = " < ";
    size_t messageLength = socket->available() ?
                           socket->read_some(asio::buffer(buffer)) :
                           0;

    if (0 != messageLength) {
        if (MessengerServer::isProtocolMessage(buffer)) {
            delimiter = " >> ";
        }
        std::cout << endPoint
                  << delimiter << buffer << std::endl;
        return true;
    }
    return false;
}