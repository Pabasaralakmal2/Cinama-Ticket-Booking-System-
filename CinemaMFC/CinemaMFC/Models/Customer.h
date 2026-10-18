#pragma once
#include "User.h"
#include <string>
class Customer : public User {
private:
    std::string fullName;
    std::string phone;
    std::string email;
public:
    Customer(int id = 0, const std::string& uname = "", 
             const std::string& name = "", const std::string& ph = "", const std::string& mail = "")
        : User(id, uname, "customer"), fullName(name), phone(ph), email(mail) {}
    std::string GetFullName() const { return fullName; }
    std::string GetPhone() const { return phone; }
    std::string GetEmail() const { return email; }
    void SetFullName(const std::string& name) { fullName = name; }
    void SetPhone(const std::string& ph) { phone = ph; }
    void SetEmail(const std::string& mail) { email = mail; }
    std::string GetProfileSummary() const override {
        return "Customer: " + fullName + " | Contact: " + phone;
    }
};
