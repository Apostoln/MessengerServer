#include <easylogging++.h>

#include "MessengerServer.hpp"
#include "utils.hpp"

INITIALIZE_EASYLOGGINGPP //crutch for logger

constexpr static unsigned short PORT = 1234;

int main() {

    el::Level logLevel = el::Level::Debug;
    bool isConsoleOut = true;
    std::string loggingFile = "/home/portaone/Workspace/proj/CLion/Messenger/MessengerServer";

    configureLogger(isConsoleOut, loggingFile, logLevel);

    MessengerServer messengerServer(PORT);
    messengerServer.run();
    return 0;
}