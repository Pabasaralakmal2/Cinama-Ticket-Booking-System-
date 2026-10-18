#include "pch.h"
#include "BookingRepository.h"
#include "vendor/json.hpp"
#include <iostream>
#include <random>
#include <ctime>
#include <iomanip>
#include <sstream>
using json = nlohmann::json;
static int jInt(const json& obj, const std::string& key) {
    if (obj.contains(key) && obj[key].is_number()) return obj[key].get<int>();
    return 0;
}
std::string BookingRepository::GenerateBookingReference(const std::string& hallId) {
    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << "NEX-" << hallId << "-" << std::put_time(&tm, "%Y%m%d");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100000, 999999);
    oss << "-" << dist(gen);
    return oss.str();
}
bool BookingRepository::CreateBooking(Booking& booking) {
    booking.SetBookingReference(GenerateBookingReference("CX"));
    json bookingBody;
    bookingBody["booking_reference"] = booking.GetBookingReference();
    if (booking.GetCustomerId() > 0) {
        bookingBody["customer_id"] = booking.GetCustomerId();
    }
    bookingBody["showtime_id"]       = booking.GetShowtimeId();
    bookingBody["total_amount"]      = booking.GetTotalAmount();
    bookingBody["payment_status"]    = booking.GetPaymentStatus();
    bookingBody["booking_status"]    = booking.GetBookingStatus();
    std::string resp = DatabaseManager::GetInstance().HttpPost("bookings", bookingBody.dump());
    if (resp.empty()) {
        std::cerr << "BookingRepository: Failed to insert booking\n";
        return false;
    }
    int newBookingId = 0;
    try {
        auto arr = json::parse(resp);
        if (!arr.empty()) newBookingId = jInt(arr[0], "booking_id");
    } catch (...) {}
    if (newBookingId == 0) {
        std::cerr << "BookingRepository: Could not get new booking_id\n";
        return false;
    }
    booking.SetBookingId(newBookingId);
    for (const auto& seatIdStr : booking.GetSeatCodes()) {
        json seatBody;
        try {
            seatBody["booking_id"] = newBookingId;
            seatBody["seat_id"]    = std::stoi(seatIdStr);
        } catch (...) {
            seatBody["booking_id"] = newBookingId;
            seatBody["seat_code"]  = seatIdStr; 
        }
        std::string seatResp = DatabaseManager::GetInstance().HttpPost("booking_seats", seatBody.dump());
        if (seatResp.empty()) {
            std::cerr << "BookingRepository: Seat conflict or error for seat " << seatIdStr << "\n";
            DatabaseManager::GetInstance().HttpDelete("bookings", "booking_id=eq." + std::to_string(newBookingId));
            return false;
        }
    }
    json payBody;
    payBody["booking_id"]      = newBookingId;
    payBody["amount"]          = booking.GetTotalAmount();
    payBody["payment_method"]  = "Card";
    payBody["payment_status"]  = booking.GetPaymentStatus();
    std::string payResp = DatabaseManager::GetInstance().HttpPost("payments", payBody.dump());
    if (payResp.empty()) {
        std::cerr << "BookingRepository: Failed to insert payment\n";
        DatabaseManager::GetInstance().HttpDelete("bookings", "booking_id=eq." + std::to_string(newBookingId));
        return false;
    }
    return true;
}
void BookingRepository::ClearOldBookings() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    std::string todayStr = oss.str();
    DatabaseManager::GetInstance().HttpDelete("bookings", "created_at=lt." + todayStr);
}
