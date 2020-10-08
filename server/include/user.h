//
// Created by GUY on 08/10/2020.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <string>

class User {
private:
    std::string name;
    bool admin;

public:
    User(std::string name);
    ~User();

public:
    bool is_admin() { return this->admin; }

};

#endif //SERVER_USER_H
