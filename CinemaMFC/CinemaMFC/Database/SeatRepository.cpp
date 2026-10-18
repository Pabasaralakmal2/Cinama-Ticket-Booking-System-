#include "pch.h"
#include "SeatRepository.h"
#include "vendor/json.hpp"
#include <iostream>
using json = nlohmann::json;
static std::string jStr(const json& obj, const std::string& key) {
    if (obj.contains(key) && obj[key].is_string()) return obj[key].get<std::string>();
    return "";
}
static int jInt(const json& obj, const std::string& key) {
    if (obj.contains(key) && obj[key].is_number()) return obj[key].get<int>();
    return 0;
}
std::vector<Seat> SeatRepository::GetSeatsForHall(const std::string& hallId) {
    std::vector<Seat> seats;
    std::string query = "select=seat_id,hall_id,row_label,seat_number,seat_code"
                        "&hall_id=eq." + hallId +
                        "&order=row_label,seat_number";
    std::string resp = DatabaseManager::GetInstance().HttpGet("seats", query);
    if (resp.empty()) return seats;
    try {
        auto arr = json::parse(resp);
        for (auto& obj : arr) {
            Seat s;
            s.SetSeatId    (jInt(obj, "seat_id"));
            s.SetHallId    (jStr(obj, "hall_id"));
            s.SetRowLabel  (jStr(obj, "row_label"));
            s.SetSeatNumber(jInt(obj, "seat_number"));
            s.SetSeatCode  (jStr(obj, "seat_code"));
            seats.push_back(s);
        }
    } catch (...) {
        std::cerr << "SeatRepository: JSON parse error\n";
    }
    return seats;
}
std::set<int> SeatRepository::GetBookedSeatIdsForShowtime(int showtimeId) {
    std::set<int> bookedSeats;
    std::string query =
        "select=seat_id,bookings!inner(showtime_id,booking_status)"
        "&bookings.showtime_id=eq." + std::to_string(showtimeId) +
        "&bookings.booking_status=eq.Active";
    std::string resp = DatabaseManager::GetInstance().HttpGet("booking_seats", query);
    if (resp.empty()) return bookedSeats;
    try {
        auto arr = json::parse(resp);
        for (auto& obj : arr) {
            bookedSeats.insert(jInt(obj, "seat_id"));
        }
    } catch (...) {
        std::cerr << "SeatRepository: GetBookedSeatIds JSON parse error\n";
    }
    return bookedSeats;
}
