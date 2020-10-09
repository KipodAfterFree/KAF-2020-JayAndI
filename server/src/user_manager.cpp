//
// Created by GUY on 08/10/2020.
//

#include <iostream>
#include <experimental/filesystem>
#include "../include/user_manager.h"
#include "../include/common.h"
#include <boost/algorithm/string.hpp>

bool UserManager::add_user(const std::string &name, const std::string &password, bool admin) {
    if (users.capacity() <= users.size())
        return false;

    users.insert(users.begin(), std::make_unique<User *>(new User{name, password, admin}));
    return true;
}

void UserManager::print_users() {
    for (auto &e : users) {
        if (e == nullptr)
            continue;
        auto user = static_cast<User *>(*e);
        std::cout << *user << std::endl;
    }
}

void UserManager::initialize() {
    namespace fs = std::experimental::filesystem;

    auto cur_dir{fs::current_path()};
    auto users_path = cur_dir.parent_path() / "res" / "users" / "users.txt";

    std::uintmax_t file_size{get_file_size(users_path)};

    auto users_content = new char[file_size + 1]{};
    read_file(users_path, users_content);

    std::vector <std::string> split_lines;

    boost::split(split_lines, users_content, boost::is_any_of("\n"));

    for (auto &line : split_lines) {
        std::vector <std::string> split_commas;
        boost::split(split_commas, line, boost::is_any_of(","));

        if (split_commas.size() < 3) {
            std::ostringstream error_stream;
            error_stream << "Not enough columns in users.txt: " << line << std::endl;
            throw std::out_of_range{error_stream.str()};
        }

        this->add_user(split_commas[0], split_commas[1],
                       boost::algorithm::to_lower_copy(split_commas[2]) == "true");
    }
}

bool UserManager::remove_user(const std::string &name, const std::string &password) {
    for (auto i = this->users.begin(); i < this->users.end(); ++i) {
        User *user = **i;
        if (user == nullptr)
            continue;

        if (user->get_name() == name) {
            if (user->get_password() == password) {
                this->users.erase(i);
                return true;
            }

            // TODO: Add "Bad password" message
            return false;
        }
    }

    // TODO: Add "User does not exist" message
    return false;
}

bool UserManager::login_user(const std::string &name, const std::string &password) {
    for (auto &cur : this->users) {
        auto user = static_cast<User *>(*cur);
        if (user->get_name() == name) {
            return user->get_password() == password;
        }
    }
    // Couldn't find user
    return false;
}

bool UserManager::register_user(const std::string &name, const std::string &password) {
    for (auto &cur : this->users) {
        auto user = static_cast<User *>(*cur);
        if (user->get_name() == name) {
            // Found user. Can't register again
            return false;
        }
    }
    this->add_user(name, password, false);
    return true;
}

