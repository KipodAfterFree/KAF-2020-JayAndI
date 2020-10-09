#include <httplib.h>
#include <iomanip>
#include "../include/page_manager.h"
#include "../include/user_manager.h"

#define PORT 12345

std::ostream &operator<<(std::ostream &os, const std::multimap <std::string, std::string> &mmap) {
    unsigned int max = mmap.size();
    unsigned int cur = 0;
    for (const auto &i : mmap) {
        os << i.first << "=" << i.second;
        if (cur < max - 1)
            os << "&";

        ++cur;
    }
    return os;
}

int main(int argc, char *argv[]) {

    const auto &user_manager{new UserManager(10)};

    // user_manager->add_user("michael", "mykull", true);
    // user_manager->add_user("mukul", "bla", false);

    // user_manager->remove_user("michael", "mykull");
    // user_manager->register_user("michael", "mykull");

    // user_manager->print_users();

    const auto &page_manager{new PageManager};

    httplib::Server svr;

    // TODO: Change to POST
    svr.Get("/login", [&user_manager, &page_manager](const httplib::Request &req, httplib::Response &res) {
        std::string username{req.get_param_value("username")};
        std::string password{req.get_param_value("password")};
        Page *show_page;

        if (username.empty() || password.empty() || !user_manager->login_user(username, password)) {
            res.status = 400;
            show_page = page_manager->get_page("oops");
        } else {
            res.status = 200;
            show_page = page_manager->get_page("login");
        }

        res.set_content(show_page->get_content(), "text/html");
    });

    svr.Get("/delete", [&user_manager, &page_manager](const httplib::Request &req, httplib::Response &res) {
        std::string username{req.get_param_value("username")};
        std::string password{req.get_param_value("password")};
        Page *show_page;

        if (username.empty() || password.empty() || !user_manager->remove_user(username, password)) {
            res.status = 400;
            show_page = page_manager->get_page("oops");
        } else {
            res.status = 200;
            show_page = page_manager->get_page("home");
        }

        res.set_content(show_page->get_content(), "text/html");
    });

    svr.Get("/register", [&user_manager, &page_manager](const httplib::Request &req, httplib::Response &res) {
        std::string username{req.get_param_value("username")};
        std::string password{req.get_param_value("password")};
        std::string age{req.get_param_value("age")};
        Page *show_page;

        if (username.empty() || password.empty() || age.empty() ||
            !user_manager->register_user(username, password, age)) {
            res.status = 400;
            show_page = page_manager->get_page("oops");
        } else {
            res.status = 200;
            show_page = page_manager->get_page("home");
        }

        res.set_content(show_page->get_content(), "text/html");
    });

    svr.Get(R"(.*)", [](const httplib::Request &req, httplib::Response &res) {
        res.status = 404;
        res.set_content("", "text/html");
    });

    svr.set_logger([](const httplib::Request &req, const httplib::Response &res) {
        std::cout << "[" << req.remote_addr << "] " << req.method << " " << req.path;
        if (!req.params.empty())
            std::cout << "?" << req.params;
        std::cout << std::endl;
    });

    std::cout << "The server is running on port: " << std::to_string(PORT) << std::endl;

    svr.listen("0.0.0.0", PORT);
    return 0;
}
