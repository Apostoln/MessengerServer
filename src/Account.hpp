#ifndef MESSENGERSERVER_ACCOUNT_HPP
#define MESSENGERSERVER_ACCOUNT_HPP

#include <string>

struct Account {
    std::string login;
    std::string password;
    bool isOnline = false;
};


#endif //MESSENGERSERVER_ACCOUNT_HPP
