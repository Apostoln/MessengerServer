#include <argparse.hpp>
#include <easylogging++.h>

#include "MessengerServer.hpp"
#include "utils.hpp"
#include "constants.hpp"

INITIALIZE_EASYLOGGINGPP //crutch for logger

constexpr static unsigned short PORT = 1234;

int main(int argc, const char* argv[]) {
    ArgumentParser parser;
    parser.addArgument("-p", "--port", 1);
    parser.addArgument("-l", "--logLevel", 1);
    parser.addArgument("-c", "--cout", 1);
    parser.addArgument("-f", "--fileLogger", 1);
    parser.addArgument("-a", "--accounts", 1);
    parser.parse(argc, argv);

    auto portArg = parser.retrieve<std::string>("port");
    auto logLevel = getLogLevel(parser.retrieve<std::string>("logLevel"));
    bool isConsoleOut = !parser.retrieve<std::string>("cout").empty();
    auto pathToAccounts = parser.retrieve<std::string>("accounts");
    auto loggingFile = parser.retrieve<std::string>("fileLogger");

    if (el::Level::Unknown == logLevel) {
        LOG(DEBUG) << "Log level is not specified. Using default log level: " << DEFAULT_LOG_LEVEL;
        logLevel = getLogLevel(DEFAULT_LOG_LEVEL);
    }
    if (loggingFile.empty()) {
        LOG(DEBUG) << "Log file is not specified. Default path to log file is used: " << DEFAULT_LOG_FILE_PATH;
        loggingFile = DEFAULT_LOG_FILE_PATH;
    }
    if (pathToAccounts.empty()) {
        LOG(DEBUG) << "Path to file with accounts is not specified. "
                   << "Trying to use default path " << DEFAULT_PATH_TO_ACCOUNTS;
        pathToAccounts = DEFAULT_PATH_TO_ACCOUNTS;
    }

    logLevel = el::Level::Debug;
    isConsoleOut = true;
    loggingFile = "/home/portaone/Workspace/proj/CLion/Messenger/MessengerServer/log/messenger.log";

    configureLogger(isConsoleOut, loggingFile, logLevel);

    pathToAccounts = "/home/portaone/Workspace/proj/CLion/Messenger/MessengerServer/etc/accounts.txt";
    Registrar* registrar = new Registrar(pathToAccounts);

    MessengerServer messengerServer(registrar, PORT);
    messengerServer.run();
    return 0;
}