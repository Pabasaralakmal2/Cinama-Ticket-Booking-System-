#pragma once
#include <string>
#include <iostream>

class User {
protected:
    std::string id;
    std::string name;

public:
    User(std::string id, std::string name) : id(id), name(name) {}
    virtual ~User() {}

    virtual std::string getRole() const = 0;

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    void setName(const std::string& n) { name = n; }
};

class Customer : public User {
private:
    std::string phone;
    std::string email;

public:
    Customer(std::string id, std::string name, std::string phone, std::string email)
        : User(id, name), phone(phone), email(email) {}

    std::string getRole() const override {
        return "Customer";
    }

    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }
};

class Admin : public User {
private:
    std::string username;

public:
    Admin(std::string id, std::string name, std::string username)
        : User(id, name), username(username) {}

    std::string getRole() const override {
        return "Admin";
    }

    std::string getUsername() const { return username; }
};
