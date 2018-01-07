#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <memory>

#include "Registrar.hpp"

Registrar::Registrar(const std::string& source_)
    : source(source_)
{
    std::cout << "Registrar created" << std::endl;
    download();
}


Registrar::Registrar(std::string&& source_)
    : source(std::move(source_))
{
    std::cout << "Registrar created" << std::endl;
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
        std::cout << "Accounts list is empty" << std::endl;
    }
    else {
        std::cout << accounts.size() << " accounts downloaded" << std::endl;
        for (auto account: accounts) {
            std::cout << account.login << " " << account.password << std::endl;
        }
    }
}

void Registrar::upload() {
    //rewrite file
    std::ofstream fout(source);
    if(!fout.is_open()) {
        exit(2);
    }
    std::cout << accounts.size() << " accounts uploaded:";
    for(auto account: accounts) {
        auto accountString = account.login + " " + account.password;
        fout << accountString << std::endl;
    }
}

void Registrar::addAccount(Account acc) {
    accounts.push_back(acc);
}

std::pair<bool, std::shared_ptr<Account>>
Registrar::authAccount(Account acc) {
    auto foundAccount = std::find_if(accounts.begin(), accounts.end(), [&](Account el){
        return el.login == acc.login;
    });
    bool isAuth  = foundAccount != accounts.end() &&
                   foundAccount->isOnline == false &&
                   foundAccount->password == acc.password;
    std::shared_ptr<Account> accountPtr = nullptr;
    if (isAuth){
        accountPtr.reset(&(*foundAccount));
    }

    return std::make_pair<bool, std::shared_ptr<Account>>(std::move(isAuth),
                                                          std::move(accountPtr));

}