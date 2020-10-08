//
// Created by GUY on 08/10/2020.
//

#include "../include/page.h"
#include <fstream>
#include <iostream>


Page::Page(fs::path &fname) {
    std::fstream page_file {fname.c_str()};

    std::error_code error_code;
    std::uintmax_t file_size;

    if (!page_file) {
        std::cout << "Could not open handle to page: " << fname << std::endl;
        exit(1);
    }

    file_size = fs::file_size(fname, error_code);

    if (error_code.value() != 0) {
        std::cout << "Could not get page size: " << fname << std::endl;
        std::cout << "Error: " << error_code.message() << std::endl;
        exit(1);
    }

    this->content = std::make_unique<char*>(new char[file_size]());

    page_file.read(*this->content.get(), file_size);

    page_file.close();
}
