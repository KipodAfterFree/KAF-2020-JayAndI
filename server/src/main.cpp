#include <httplib.h>
#include <iomanip>
#include "../include/page_manager.h"
#include "../include/user_manager.h"

#define PORT 12345

int main(int argc, char* argv[]) {

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
        Page *show_page;

        if (username.empty() || password.empty() || !user_manager->register_user(username, password)) {
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

    std::cout << "The server is running on port: " << std::to_string(PORT) << std::endl;

    svr.listen("0.0.0.0", PORT);
    return 0;
}
