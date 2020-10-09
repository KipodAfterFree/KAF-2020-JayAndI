//
// Created by GUY on 08/10/2020.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <string>
#include <ostream>

class User {
private:
    std::string name;
        std::string password;
        bool admin;

        public:
        User(const std::string& _name, const std::string& _password, bool _admin=false): name{ _name }, admin{ _admin }, password(_password) {};
        ~User() = default;

        public:
        inline bool is_admin() const { return this->admin; }
        inline std::string get_name() const { return this->name; }
        inline std::string get_password() const { return this->password; }

        public:
        friend std::ostream& operator<<(std::ostream& out, User user);
};


#endif //SERVER_USER_H
