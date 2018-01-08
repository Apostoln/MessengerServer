#include <easylogging++.h>

#include "MessengerServer.hpp"
#include "utils.hpp"

INITIALIZE_EASYLOGGINGPP //crutch for logger

constexpr static unsigned short PORT = 1234;

int main() {

    el::Level logLevel = el::Level::Debug;
    bool isConsoleOut = true;
    std::string loggingFile = "/home/portaone/Workspace/proj/CLion/Messenger/MessengerServer/log/messenger.log";

    configureLogger(isConsoleOut, loggingFile, logLevel);

    std::string accountPath = "/home/portaone/Workspace/proj/CLion/Messenger/MessengerServer/etc/accounts.txt";
    Registrar* registrar = new Registrar(accountPath);

    MessengerServer messengerServer(registrar, PORT);
    messengerServer.run();
    return 0;
}