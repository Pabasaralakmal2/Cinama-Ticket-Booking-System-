#pragma once
#include <string>
class Payment {
protected:
    int paymentId;
    int bookingId;
    double amount;
    std::string paymentStatus;
public:
    Payment(int bId, double amt) : paymentId(0), bookingId(bId), amount(amt), paymentStatus("Pending") {}
    virtual ~Payment() {}
    virtual bool ProcessPayment() = 0;
    int GetPaymentId() const { return paymentId; }
    int GetBookingId() const { return bookingId; }
    double GetAmount() const { return amount; }
    std::string GetPaymentStatus() const { return paymentStatus; }
    void SetPaymentId(int id) { paymentId = id; }
    void SetPaymentStatus(const std::string& status) { paymentStatus = status; }
};
