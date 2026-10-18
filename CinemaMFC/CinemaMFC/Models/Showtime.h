#pragma once
#include <string>
class Showtime {
private:
    int showtimeId;
    int movieId;
    std::string hallId;
    std::string showDate;
    std::string startTime;
    std::string format;
    double ticketPrice;
public:
    Showtime() : showtimeId(0), movieId(0), ticketPrice(0.0) {}
    int GetShowtimeId() const { return showtimeId; }
    int GetMovieId() const { return movieId; }
    std::string GetHallId() const { return hallId; }
    std::string GetShowDate() const { return showDate; }
    std::string GetStartTime() const { return startTime; }
    std::string GetFormat() const { return format; }
    double GetTicketPrice() const { return ticketPrice; }
    void SetShowtimeId(int id) { showtimeId = id; }
    void SetMovieId(int id) { movieId = id; }
    void SetHallId(const std::string& id) { hallId = id; }
    void SetShowDate(const std::string& date) { showDate = date; }
    void SetStartTime(const std::string& time) { startTime = time; }
    void SetFormat(const std::string& f) { format = f; }
    void SetTicketPrice(double price) { ticketPrice = price; }
};
