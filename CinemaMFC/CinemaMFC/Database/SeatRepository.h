#pragma once
#include "Seat.h"
#include "DatabaseManager.h"
#include <vector>
#include <string>
#include <set>
class SeatRepository {
public:
    SeatRepository() {}
    ~SeatRepository() {}
    std::vector<Seat> GetSeatsForHall(const std::string& hallId);
    std::set<int> GetBookedSeatIdsForShowtime(int showtimeId);
};
