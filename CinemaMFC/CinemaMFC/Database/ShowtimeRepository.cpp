#include "pch.h"
#include "ShowtimeRepository.h"
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
static double jDouble(const json& obj, const std::string& key) {
    if (obj.contains(key) && obj[key].is_number()) return obj[key].get<double>();
    return 0.0;
}
static std::vector<Showtime> parseShowtimes(const std::string& jsonStr) {
    std::vector<Showtime> result;
    if (jsonStr.empty()) return result;
    try {
        auto arr = json::parse(jsonStr);
        for (auto& obj : arr) {
            Showtime s;
            s.SetShowtimeId (jInt   (obj, "showtime_id"));
            s.SetMovieId    (jInt   (obj, "movie_id"));
            s.SetHallId     (jStr   (obj, "hall_id"));
            s.SetShowDate   (jStr   (obj, "show_date"));
            s.SetStartTime  (jStr   (obj, "start_time"));
            s.SetFormat     (jStr   (obj, "format"));
            s.SetTicketPrice(jDouble(obj, "ticket_price"));
            result.push_back(s);
        }
    } catch (...) {
        std::cerr << "ShowtimeRepository: JSON parse error\n";
    }
    return result;
}
bool ShowtimeRepository::AddShowtime(const Showtime& showtime) {
    if (!ValidateFormatForHall(showtime.GetHallId(), showtime.GetFormat())) {
        std::cerr << "Validation Failed: Cannot show " << showtime.GetFormat()
                  << " in Hall " << showtime.GetHallId() << std::endl;
        return false;
    }
    json body;
    body["movie_id"]     = showtime.GetMovieId();
    body["hall_id"]      = showtime.GetHallId();
    body["show_date"]    = showtime.GetShowDate();
    body["start_time"]   = showtime.GetStartTime();
    body["format"]       = showtime.GetFormat();
    body["ticket_price"] = showtime.GetTicketPrice();
    std::string resp = DatabaseManager::GetInstance().HttpPost("showtimes", body.dump());
    return !resp.empty();
}
std::vector<Showtime> ShowtimeRepository::GetAllShowtimes() {
    std::string resp = DatabaseManager::GetInstance().HttpGet(
        "showtimes",
        "select=showtime_id,movie_id,hall_id,show_date,start_time,format,ticket_price"
    );
    return parseShowtimes(resp);
}
std::vector<Showtime> ShowtimeRepository::GetShowtimesByMovieId(int movieId) {
    std::string query = "select=showtime_id,movie_id,hall_id,show_date,start_time,format,ticket_price"
                        "&movie_id=eq." + std::to_string(movieId);
    std::string resp  = DatabaseManager::GetInstance().HttpGet("showtimes", query);
    return parseShowtimes(resp);
}
bool ShowtimeRepository::DeleteShowtime(int id) {
    return DatabaseManager::GetInstance().HttpDelete("showtimes", "showtime_id=eq." + std::to_string(id));
}
bool ShowtimeRepository::ValidateFormatForHall(const std::string& hallId, const std::string& format) {
    if (format == "3D" && hallId == "C3") return false;
    return true;
}
