#include <httplib.h>
#include <iomanip>
#include "../include/page_manager.h"


int main(int argc, char* argv[])
{

    const auto& manager {new PageManager};

    httplib::Server svr;

    svr.Post("/login", [&manager](const httplib::Request &req, httplib::Response &res) {
        std::string test {req.get_param_value("hello")};

        res.set_content("hello" , "text/html");
    });

    svr.Post("/register", [&manager](const httplib::Request &req, httplib::Response &res) {
        std::string username {req.get_param_value("username")};
        std::string password {req.get_param_value("password")};

        res.set_content("hello" , "text/html");
    });

    svr.Get(R"(.*)", [](const httplib::Request &req, httplib::Response &res) {
        res.status = 404;
        res.set_content("", "text/html");
    });

    std::cout << "The server is running on port: " << std::to_string(8080) << std::endl;

    svr.listen("0.0.0.0", 8080);
    return 0;
}
