//
// Created by GUY on 08/10/2020.
//

#include <iostream>
#include <experimental/filesystem>
#include "../include/user_manager.h"
#include "../include/common.h"
#include <boost/algorithm/string.hpp>

bool UserManager::add_user(const std::string &name, const std::string &password, unsigned short age, bool admin) {
    if (users.capacity() <= users.size())
        return false;

    users.insert(users.begin(), std::make_unique<User *>(new User{name, password, age, admin}));
    return true;
}

[[maybe_unused]] void UserManager::print_users() {
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

        if (split_commas.size() != 4) {
            std::ostringstream error_stream;
            error_stream << "Not enough columns in users.txt: " << line << std::endl;
            throw std::out_of_range{error_stream.str()};
        }
        auto age = static_cast<unsigned short>(std::stoi(split_commas[2]));
        this->add_user(split_commas[0], split_commas[1], age,
                       boost::algorithm::to_lower_copy(split_commas[3]) == "true");
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

bool UserManager::register_user(const std::string &name, const std::string &password, const std::string &age) {
    for (auto &cur : this->users) {
        auto user = static_cast<User *>(*cur);
        if (user->get_name() == name) {
            // Found user. Can't register again
            return false;
        }
    }
    auto _age = static_cast<unsigned short>(std::stoi(age));
    this->add_user(name, password, _age, false);
    return true;
}

bool UserManager::is_admin(const std::string &name) {
    for (auto &cur : this->users) {
        auto user = static_cast<User *>(*cur);
        if (user->get_name() == name) {
            return user->is_admin();
        }
    }
    // Couldn't find user
    return false;}

std::ostringstream UserManager::get_users() {
    std::ostringstream out;
    unsigned int max = this->users.size();
    unsigned int cur_index = 0;

    for (auto &cur : this->users) {
        auto user = static_cast<User *>(*cur);

        out << user->get_name();
        if (cur_index < max - 1)
            out << ",";

        ++cur_index;
    }

    return out;
}

bool UserManager::user_exists(const std::string& name) {
    for (auto &cur : this->users) {
        auto user = static_cast<User *>(*cur);

        if (user->get_name() == name)
            return true;
    }
    return false;
}

void UserManager::add_to_alert_queue(const std::string &target, FILE *tmpf) {
    auto target_user = this->alerts.find(target);
    if (target_user == this->alerts.end()) {
        // Create user alert queue
        auto alert_queue = new std::queue<FILE*>();
        this->alerts[target] = static_cast<std::queue<FILE*>*>(alert_queue);
    } else {
        this->alerts[target]->push(tmpf);
    }
}

