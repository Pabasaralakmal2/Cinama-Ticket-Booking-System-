#pragma once
#include <string>
#include <vector>
class Booking {
private:
    int bookingId;
    std::string bookingReference;
    int customerId;
    int showtimeId;
    std::string bookingDate;
    double totalAmount;
    std::string paymentStatus;
    std::string bookingStatus;
    std::vector<std::string> seatCodes; 
public:
    Booking() : bookingId(0), customerId(0), showtimeId(0), totalAmount(0.0) {}
    int GetBookingId() const { return bookingId; }
    std::string GetBookingReference() const { return bookingReference; }
    int GetCustomerId() const { return customerId; }
    int GetShowtimeId() const { return showtimeId; }
    std::string GetBookingDate() const { return bookingDate; }
    double GetTotalAmount() const { return totalAmount; }
    std::string GetPaymentStatus() const { return paymentStatus; }
    std::string GetBookingStatus() const { return bookingStatus; }
    std::vector<std::string> GetSeatCodes() const { return seatCodes; }
    void SetBookingId(int id) { bookingId = id; }
    void SetBookingReference(const std::string& ref) { bookingReference = ref; }
    void SetCustomerId(int id) { customerId = id; }
    void SetShowtimeId(int id) { showtimeId = id; }
    void SetBookingDate(const std::string& date) { bookingDate = date; }
    void SetTotalAmount(double amount) { totalAmount = amount; }
    void SetPaymentStatus(const std::string& status) { paymentStatus = status; }
    void SetBookingStatus(const std::string& status) { bookingStatus = status; }
    void AddSeatCode(const std::string& code) { seatCodes.push_back(code); }
    void SetSeatCodes(const std::vector<std::string>& codes) { seatCodes = codes; }
};
