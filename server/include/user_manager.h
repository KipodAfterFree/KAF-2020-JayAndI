//
// Created by GUY on 08/10/2020.
//
#include <memory>
#include <vector>
#include "user.h"


#ifndef SERVER_USER_MANAGER_H
#define SERVER_USER_MANAGER_H

class UserManager{
        private:
        std::vector < std::unique_ptr < User * >> users;

        private:
        void initialize();

        public:
        UserManager(size_t user_length): users{} {
            users.reserve(user_length);
            this->initialize();
        }
        ~UserManager() = default;

        public:
        bool add_user(const std::string& name, const std::string& password, unsigned short age, bool admin);
        void print_users();

        public:
        bool remove_user(const std::string& name, const std::string& password);
        bool login_user(const std::string& name, const std::string& password);
        bool register_user(const std::string& name, const std::string& password, const std::string& age);


};


#endif //SERVER_USER_MANAGER_H
