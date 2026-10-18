#pragma once
#include <string>
#include <iostream>

// ABSTRACTION & POLYMORPHISM DEMONSTRATION

class Payment {
protected:
    std::string paymentId;
    double amount;

public:
    Payment(std::string id, double amt) : paymentId(id), amount(amt) {}
    virtual ~Payment() {}

    // Pure virtual method (Abstraction)
    virtual bool processPayment() = 0; 
    
    double getAmount() const { return amount; }
};

class CardPayment : public Payment {
private:
    std::string cardNumber;
    std::string cardHolder;
    std::string expiry;
    std::string cvv; // Stored temporarily only for processing simulation

public:
    CardPayment(std::string id, double amt, std::string cNum, std::string holder, std::string exp, std::string c)
        : Payment(id, amt), cardNumber(cNum), cardHolder(holder), expiry(exp), cvv(c) {}

    // Overriding the virtual method (Polymorphism)
    bool processPayment() override {
        // Simulate payment processing logic here
        // E.g., check if card number is 16 digits
        if (cardNumber.length() == 16 && cvv.length() == 3) {
            std::cout << "Processing LKR " << amount << " via Card..." << std::endl;
            return true; // Success
        }
        return false; // Failed
    }
};
