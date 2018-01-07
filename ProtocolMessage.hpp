#ifndef MESSENGERSERVER_PROTOCOLMESSAGE_HPP
#define MESSENGERSERVER_PROTOCOLMESSAGE_HPP

#include <string>
#include <unordered_map>

enum class ProtocolMessage {
    START,
    OK,
    ERROR,
    CANCEL,
    REG,
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

static std::unordered_map<ProtocolMessage, std::string> protocolString  {
        {ProtocolMessage::START, "#START"},
        {ProtocolMessage::OK, "#OK"},
        {ProtocolMessage::ERROR, "#ERROR"},
        {ProtocolMessage::CANCEL, "#CANCEL"},
        {ProtocolMessage::REG, "#REG"},
        {ProtocolMessage::UNKNOWN, "UNKNOWN COMMAND"}
};


bool operator==(ProtocolMessage msg, const std::string& str);

bool operator==(const std::string& str, ProtocolMessage msg);

ProtocolMessage fromString(const std::string& msg);

#endif //MESSENGERSERVER_PROTOCOLMESSAGE_HPP
