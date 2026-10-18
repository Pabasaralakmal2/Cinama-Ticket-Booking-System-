#pragma once
#include <string>
#include <vector>
class Ticket {
private:
    std::string bookingReference;
    std::string movieTitle;
    std::string showDate;
    std::string startTime;
    std::string hallName;
    std::string format;
    std::vector<std::string> seatCodes;
    std::string customerName;
    double totalAmount;
    std::string qrCodeData; 
public:
    Ticket() : totalAmount(0.0) {}
    std::string GetBookingReference() const { return bookingReference; }
    std::string GetMovieTitle() const { return movieTitle; }
    std::string GetShowDate() const { return showDate; }
    std::string GetStartTime() const { return startTime; }
    std::string GetHallName() const { return hallName; }
    std::string GetFormat() const { return format; }
    std::vector<std::string> GetSeatCodes() const { return seatCodes; }
    std::string GetCustomerName() const { return customerName; }
    double GetTotalAmount() const { return totalAmount; }
    std::string GetQrCodeData() const { return qrCodeData; }
    void SetBookingReference(const std::string& ref) { bookingReference = ref; }
    void SetMovieTitle(const std::string& title) { movieTitle = title; }
    void SetShowDate(const std::string& date) { showDate = date; }
    void SetStartTime(const std::string& time) { startTime = time; }
    void SetHallName(const std::string& name) { hallName = name; }
    void SetFormat(const std::string& f) { format = f; }
    void SetSeatCodes(const std::vector<std::string>& seats) { seatCodes = seats; }
    void SetCustomerName(const std::string& name) { customerName = name; }
    void SetTotalAmount(double amount) { totalAmount = amount; }
    void SetQrCodeData(const std::string& qrData) { qrCodeData = qrData; }
    void GenerateQrCodeData() {
        qrCodeData = "NEXORA TICKET\n"
                     "ID: " + bookingReference + "\n"
                     "Movie: " + movieTitle + "\n"
                     "Date: " + showDate + " " + startTime + "\n"
                     "Hall: " + hallName;
    }
};
