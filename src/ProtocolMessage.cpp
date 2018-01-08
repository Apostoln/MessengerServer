#include "ProtocolMessage.hpp"

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