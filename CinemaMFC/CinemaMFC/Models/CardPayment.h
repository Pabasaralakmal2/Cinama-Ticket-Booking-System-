#pragma once
#include "Payment.h"
#include <string>
class CardPayment : public Payment {
private:
    std::string cardNumber;
    std::string cardHolderName;
    std::string expiryDate;
public:
    CardPayment(int bId, double amt, const std::string& cNum, const std::string& name, const std::string& exp)
        : Payment(bId, amt), cardNumber(cNum), cardHolderName(name), expiryDate(exp) {}
    bool ProcessPayment() override {
        if (!cardNumber.empty() && !cardHolderName.empty()) {
            SetPaymentStatus("Success");
            return true;
        }
        SetPaymentStatus("Failed");
        return false;
    }
};
