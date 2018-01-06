#ifndef MESSENGERSERVER_PROTOCOLMESSAGE_HPP
#define MESSENGERSERVER_PROTOCOLMESSAGE_HPP

#include <string>
#include <unordered_map>

enum class ProtocolMessage {
    START,
    OK,
    ERROR,
    CANCEL,
    UNKNOWN
};

namespace std {
    template <>
    struct hash<ProtocolMessage > {
        size_t operator()(ProtocolMessage msg) const {
            return hash<int>()(static_cast<std::underlying_type<ProtocolMessage >::type>(msg));
        }
    };
}

std::unordered_map<ProtocolMessage, std::string> protocolString  {
        {ProtocolMessage::START, "#START"},
        {ProtocolMessage::OK, "#OK"},
        {ProtocolMessage::ERROR, "#ERROR"},
        {ProtocolMessage::CANCEL, "#CANCEL"},
        {ProtocolMessage::UNKNOWN, "UNKNOWN COMMAND"}
};


bool operator==(ProtocolMessage msg, const std::string& str) {
    if (protocolString[msg] == str) {
        return true;
    }
    return false;
}

bool operator==(const std::string& str, ProtocolMessage msg) {
    if (msg == str) {
        return true;
    }
    return false;
}



ProtocolMessage fromString(const std::string& msg) {
    for(auto& pair : protocolString) {
        if ( msg == pair.second) {
            return pair.first;
        }
    }
}

#endif //MESSENGERSERVER_PROTOCOLMESSAGE_HPP
