#include <easylogging++.h>

#include "MessengerServer.hpp"

INITIALIZE_EASYLOGGINGPP //crutch for logger

using namespace asio;

constexpr static unsigned short PORT = 1234;

int main() {
    MessengerServer messengerServer(PORT);
    messengerServer.run();
    return 0;
}