//
// Created by GUY on 08/10/2020.
//
#include "page.h"
#include <memory>
#include <map>

#ifndef SERVER_PAGE_MANAGER_H
#define SERVER_PAGE_MANAGER_H


class PageManager {
private:
    std::map<std::string, std::unique_ptr<Page*>> pages;
public:
    PageManager();
    ~PageManager() = default;

/*
 * Page functions
 */
public:
    void add_page(std::string name, std::string filename);
public:
    Page* get_page(std::string name);
};


#endif //SERVER_PAGE_MANAGER_H
