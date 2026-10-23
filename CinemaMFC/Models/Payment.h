#pragma once
#include <string>
#include <iostream>

class Payment {
protected:
    std::string paymentId;
    double amount;

public:
    Payment(std::string id, double amt) : paymentId(id), amount(amt) {}
    virtual ~Payment() {}

    virtual bool processPayment() = 0;

    double getAmount() const { return amount; }
};

class CardPayment : public Payment {
private:
    std::string cardNumber;
    std::string cardHolder;
    std::string expiry;
    std::string cvv;

public:
    CardPayment(std::string id, double amt, std::string cNum, std::string holder, std::string exp, std::string c)
        : Payment(id, amt), cardNumber(cNum), cardHolder(holder), expiry(exp), cvv(c) {}

    bool processPayment() override {

        if (cardNumber.length() == 16 && cvv.length() == 3) {
            std::cout << "Processing LKR " << amount << " via Card..." << std::endl;
            return true;
        }
        return false;
    }
};
