#ifndef MESSENGERSERVER_UTILS_HPP
#define MESSENGERSERVER_UTILS_HPP

#endif //MESSENGERSERVER_UTILS_HPP

#include <easylogging++.h>

void setLogLevel(el::Level level, el::Configurations& conf) {
    //emulation of logging hierarchy because implementation in easylogging++ is too crutched

    /* Logging hierarchy:
     *      el::Level::Debug
     *      el::Level::Info
     *      el::Level::Warning
     *      el::Level::Error
     */

    std::vector<el::Level> usedLevels = {el::Level::Debug, el::Level::Info, el::Level::Warning, el::Level::Error};

    for(auto l: usedLevels) {
        if (l != level) {
            conf.set(l, el::ConfigurationType::Enabled, "false");
        }
        else {
            break;
        }
    }
}

void configureLogger(bool isConsoleOut, std::string loggingFile, el::Level logLevel) {
    el::Configurations conf;
    conf.set(el::Level::Global, el::ConfigurationType::Format, "%level %datetime{%H:%m:%s}: %msg");
    conf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, isConsoleOut? "true": "false");
    if ("" != loggingFile) {
        conf.set(el::Level::Global, el::ConfigurationType::Filename, loggingFile);
    }
    setLogLevel(logLevel, conf);
    el::Loggers::reconfigureAllLoggers(conf);

    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

    LOG(DEBUG) << "Logger started";
    LOG(DEBUG) << "Log is wrote to " << loggingFile;
    LOG(DEBUG) << "Log level is " << (uint)logLevel;
    LOG(DEBUG) << "Echo to stdout is " << (isConsoleOut? "" : "not ") << "specified";
}