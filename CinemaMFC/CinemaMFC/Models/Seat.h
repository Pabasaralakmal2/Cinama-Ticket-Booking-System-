#pragma once
#include <string>
class Seat {
private:
    int seatId;
    std::string hallId;
    std::string rowLabel;
    int seatNumber;
    std::string seatCode;
public:
    Seat() : seatId(0), seatNumber(0) {}
    int GetSeatId() const { return seatId; }
    std::string GetHallId() const { return hallId; }
    std::string GetRowLabel() const { return rowLabel; }
    int GetSeatNumber() const { return seatNumber; }
    std::string GetSeatCode() const { return seatCode; }
    void SetSeatId(int id) { seatId = id; }
    void SetHallId(const std::string& hid) { hallId = hid; }
    void SetRowLabel(const std::string& rl) { rowLabel = rl; }
    void SetSeatNumber(int num) { seatNumber = num; }
    void SetSeatCode(const std::string& code) { seatCode = code; }
};
