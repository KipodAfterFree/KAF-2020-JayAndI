//
// Created by GUY on 08/10/2020.
//

#include <experimental/filesystem>
#include "common.h"

#ifndef SERVER_PAGE_H
#define SERVER_PAGE_H

class Page {
private:
    std::unique_ptr<char*> content;
        public:
        Page(fs::path& fname);
        ~Page() = default;

        public:
        char* get_content() { return *this->content; };
};

#endif //SERVER_PAGE_H












/*
typedef struct {
    bool is_enabled; // 1
    short age; // 2
    bool is_admin;
    char* name;
} User;
*/
// User* user;
// name = *((char*)user + sizeof(a) + sizeof(b) + sizeof(is_admin))

/*
 * 1. Add rename functionality
 * 2. Name: \n\nadam\x00
 *
 *
 */
















