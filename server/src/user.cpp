//
// Created by GUY on 08/10/2020.
//

#include "../include/user.h"

std::ostream &operator<<(std::ostream &out, User user) {
    out << user.get_name() << ", " << user.get_password();
    return out;
}

User::User(const std::string &_name, const std::string &_password, unsigned short _age, bool _admin = false) : age{
        _age}, admin{_admin} {
    if (_name.length() >= sizeof(name) || _password.length() >= sizeof(password))
        throw std::length_error{"Name / password too big for class"};

    memset_strip_newline(_name);

    memset(password, 0, sizeof(password));
    memcpy(this->password, _password.c_str(), _password.length());
}

void User::memset_strip_newline(const std::string &_name) {
    /*
     * I hate newlines, so I just strip the first one off and replace
     * it with the character after
     */
    memset(this->name, 0, sizeof(name));

    if (_name.at(0) != '\n') {
        memcpy(this->name, _name.c_str(), _name.length());
        return;
    }

    if (_name == "\n")
        throw std::runtime_error{"Name cannot be only newline"};

    // Access name member
    char *newline_addr = (char *) this + sizeof(this->a) + sizeof(this->age) + sizeof(this->admin);

    // THIS
    memcpy(newline_addr, _name.c_str() + 1, _name.length() - 1);

    // OR THIS
    // *newline_addr = _name.at(1);
}
