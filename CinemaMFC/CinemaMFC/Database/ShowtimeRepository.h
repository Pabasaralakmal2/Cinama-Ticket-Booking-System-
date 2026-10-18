#pragma once
#include "Showtime.h"
#include "DatabaseManager.h"
#include <vector>
#include <memory>
#include <string>
class ShowtimeRepository {
public:
    ShowtimeRepository() {}
    ~ShowtimeRepository() {}
    bool AddShowtime(const Showtime& showtime);
    std::vector<Showtime> GetAllShowtimes();
    std::vector<Showtime> GetShowtimesByMovieId(int movieId);
    bool DeleteShowtime(int id);
    bool ValidateFormatForHall(const std::string& hallId, const std::string& format);
};
