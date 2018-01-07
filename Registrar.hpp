#ifndef MESSENGERSERVER_REGISTRAR_HPP
#define MESSENGERSERVER_REGISTRAR_HPP


#include <vector>
#include <string>

#include "Account.hpp"

class Registrar {
    private:
        std::string source;
        std::vector<Account> accounts;
    public:
        Registrar(const std::string& source_);
        Registrar(std::string&& source_);
        ~Registrar();
        void download();
        void upload();


};


#endif //MESSENGERSERVER_REGISTRAR_HPP
