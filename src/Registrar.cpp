#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <memory>

#include <easylogging++.h>

#include "Registrar.hpp"

Registrar::Registrar(const std::string& source_)
    : source(source_)
{
    LOG(DEBUG) << "Registrar created";
    download();
}


Registrar::Registrar(std::string&& source_)
    : source(std::move(source_))
{
    LOG(DEBUG) << "Registrar created";
    download();
}

Registrar::~Registrar() {
    upload();
}

void Registrar::download() {
    accounts.clear();
    std::ifstream fin(source);

    if(!fin.is_open()) {
        exit(2);
    }

    //read file to buffer
    std::vector<std::string> buffer((std::istream_iterator<std::string>(fin)),
                                    std::istream_iterator<std::string>());

    bool isLogin = true; //crutch
    for(int i = 0; i < buffer.size(); i += 2) {
        accounts.push_back(Account{buffer[i], buffer[i+1]});
    }

    if(accounts.empty()) {
        LOG(WARNING) << "Accounts list is empty";
    }
    else {
        LOG(DEBUG) << accounts.size() << " accounts downloaded";
        for (auto account: accounts) {
            LOG(DEBUG) << account.login << " " << account.password;
        }
    }
}

void Registrar::upload() {
    //rewrite file
    std::ofstream fout(source);
    if(!fout.is_open()) {
        exit(2);
    }
    LOG(DEBUG) << accounts.size() << " accounts uploaded:";
    for(auto account: accounts) {
        auto accountString = account.login + " " + account.password;
        fout << accountString << std::endl;
    }
}

void Registrar::addAccount(Account acc) {
    accounts.push_back(acc);
}

std::pair<bool, Account*>
Registrar::authAccount(Account acc) {
    auto foundAccount = std::find_if(accounts.begin(), accounts.end(), [&](Account el){
        return el.login == acc.login;
    });
    bool isAuth  = foundAccount != accounts.end() &&
                   foundAccount->isOnline == false &&
                   foundAccount->password == acc.password;


    return std::pair<bool, Account*>(isAuth, foundAccount.base());
}