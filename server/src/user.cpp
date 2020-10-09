//
// Created by GUY on 08/10/2020.
//

#include "../include/user.h"

std::ostream &operator<<(std::ostream &out, User user) {
    out << user.get_name() << ", " << user.get_password();
    return out;
}
