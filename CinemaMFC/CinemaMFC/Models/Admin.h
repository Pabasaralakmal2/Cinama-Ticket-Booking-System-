#pragma once
#include "User.h"
#include <string>
class Admin : public User {
public:
    Admin(int id = 0, const std::string& uname = "")
        : User(id, uname, "admin") {}
    std::string GetProfileSummary() const override {
        return "Administrator: " + username;
    }
};
