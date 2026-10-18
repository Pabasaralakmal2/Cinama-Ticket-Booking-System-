#pragma once
#include <string>
class User {
protected: 
    int userId;
    std::string username;
    std::string role;
public:
    User(int id = 0, const std::string& uname = "", const std::string& r = "user") 
        : userId(id), username(uname), role(r) {}
    virtual ~User() {}
    int GetUserId() const { return userId; }
    std::string GetUsername() const { return username; }
    std::string GetRole() const { return role; }
    void SetUserId(int id) { userId = id; }
    void SetUsername(const std::string& uname) { username = uname; }
    void SetRole(const std::string& r) { role = r; }
    virtual std::string GetProfileSummary() const {
        return "User: " + username + " (" + role + ")";
    }
};
