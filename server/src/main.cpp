#include <httplib.h>
#include <iomanip>
#include "../include/page_manager.h"
#include "../include/user_manager.h"
#include "../include/base64.h"

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

    const auto user_manager {std::make_shared<UserManager*>(new UserManager(10))};

    // user_manager->add_user("michael", "mykull", true);
    // user_manager->add_user("mukul", "bla", false);

    // user_manager->remove_user("michael", "mykull");
    // user_manager->register_user("michael", "mykull");

    // user_manager->print_users();

    const auto &page_manager{std::make_shared<PageManager*>(new PageManager)};

    httplib::Server svr;

    // TODO: Change to POST
    svr.Get("/login", [user_manager, page_manager](const httplib::Request &req, httplib::Response &res) {
        std::string username{req.get_param_value("username")};
        std::string password{req.get_param_value("password")};
        Page *show_page;

        if (username.empty() || password.empty()) {
            res.status = 400;
            res.set_content("empty parameters", "text/html");
        } else if (!(*user_manager)->login_user(username, password)) {
            res.status = 404;
            res.set_content("bad credentials", "text/html");
        } else {
            res.status = 200;
            show_page = (*page_manager)->get_page("login");
            res.set_content(show_page->get_content(), "text/html");
        }
    });

    svr.Get("/users", [user_manager](const httplib::Request &req, httplib::Response &res) {
        std::string username{req.get_param_value("username")};
        std::string password{req.get_param_value("password")};
        std::ostringstream users;
        Page *show_page;

        if (username.empty() || password.empty()) {
            res.status = 400;
            res.set_content("empty parameters", "text/html");
        } else if (!(*user_manager)->login_user(username, password)) {
            res.status = 404;
            res.set_content("bad credentials", "text/html");
        } else if (!(*user_manager)->is_admin(username)) {
            res.status = 401;
            res.set_content("you must be an admin to do this", "text/html");
        } else {
            res.status = 200;
            users = (*user_manager)->get_users();
            res.set_content(users.str(), "text/html");
        }
    });

    svr.Post("/send_image", [user_manager, page_manager](const httplib::Request &req, httplib::Response &res) {

        std::string username{req.get_file_value("username").content};
        std::string password{req.get_file_value("password").content};
        std::string target{req.get_file_value("target").content};
        std::string image{req.get_file_value("image").content};

        Page *show_page;

        if (username.empty() || password.empty() || target.empty()) {
            res.status = 400;
            res.set_content("empty parameters", "text/html");
        } else if (!(*user_manager)->login_user(username, password)) {
            res.status = 404;
            res.set_content("bad credentials", "text/html");
        } else if (!(*user_manager)->is_admin(username)) {
            res.status = 401;
            res.set_content("you must be an admin to do this", "text/html");
        } else if (!(*user_manager)->user_exists(target)) {
            res.status = 404;
            res.set_content("the target user doesn't exist", "text/html");
        } else {
            res.status = 200;
            show_page = (*page_manager)->get_page("success");
            // Add new page
            std::FILE* tmpf = std::tmpfile();
            std::fputs(image.c_str(), tmpf);

            (*user_manager)->add_to_alert_queue(target, tmpf);

            res.set_content(show_page->get_content(), "text/html");
        }

        /* char *content;
        std::unique_ptr<char*> content_encoded;
        uint32_t padded_length; */
        /*
        // Get page content
        content = show_page->get_content();
        std::uintmax_t content_len = show_page->get_len();

        // allocate base64 buffer with padding
        padded_length = 4 * ((content_len + 2) / 3) + 1;
        content_encoded = std::make_unique<char*>(new char[padded_length + 1]);
        memset(*content_encoded, 0, padded_length + 1);

        Base64::Encode(content, content_len, *content_encoded, padded_length);

        res.set_content(*content_encoded, "image/bmp");
         */
    });

    svr.Get("/delete", [user_manager, page_manager](const httplib::Request &req, httplib::Response &res) {
        std::string username{req.get_param_value("username")};
        std::string password{req.get_param_value("password")};
        Page *show_page;

        if (username.empty() || password.empty()) {
            res.status = 400;
            res.set_content("empty parameters", "text/html");
        } else if (!(*user_manager)->remove_user(username, password)) {
            res.status = 404;
            res.set_content("bad credentials", "text/html");
        } else {
            res.status = 200;
            show_page = (*page_manager)->get_page("home");
            res.set_content(show_page->get_content(), "text/html");
        }
    });

    svr.Get("/register", [user_manager, page_manager](const httplib::Request &req, httplib::Response &res) {
        std::string username{req.get_param_value("username")};
        std::string password{req.get_param_value("password")};
        std::string age{req.get_param_value("age")};
        Page *show_page;

        if (username.empty() || password.empty() || age.empty()) {
            res.status = 400;
            res.set_content("empty parameters", "text/html");
        } else if (!(*user_manager)->register_user(username, password, age)) {
            res.status = 403;
            res.set_content("user exists", "text/html");
        } else {
            res.status = 200;
            show_page = (*page_manager)->get_page("home");
            res.set_content(show_page->get_content(), "text/html");
        }
    });

    svr.Get(R"(.*)", [](const httplib::Request &req, httplib::Response &res) {
        res.status = 404;
        res.set_content("", "text/html");
    });

    svr.set_logger([](const httplib::Request &req, const httplib::Response &res) {
        std::cout << "[" << req.remote_addr << "] " << req.method << " " << req.path;
        if (!req.params.empty())
            std::cout << "?" << req.params;
        std::cout << " -> " << res.status;
        std::cout << std::endl;
    });

    std::cout << "The server is running on port: " << std::to_string(PORT) << std::endl;

    svr.listen("0.0.0.0", PORT);
    return 0;
}
