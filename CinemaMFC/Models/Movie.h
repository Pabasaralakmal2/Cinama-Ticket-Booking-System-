#pragma once
#include <string>

// ENCAPSULATION DEMONSTRATION

class Movie {
private:
    std::string movieId;
    std::string title;
    std::string genre;
    int durationMins;
    std::string format;
    std::string status;

public:
    Movie(std::string id, std::string t, std::string g, int d, std::string f, std::string s)
        : movieId(id), title(t), genre(g), durationMins(d), format(f), status(s) {}

    // Getters
    std::string getId() const { return movieId; }
    std::string getTitle() const { return title; }
    std::string getGenre() const { return genre; }
    int getDuration() const { return durationMins; }
    std::string getFormat() const { return format; }
    std::string getStatus() const { return status; }

    // Setters with validation logic (Encapsulation)
    void setDuration(int d) {
        if (d > 0) durationMins = d;
    }
};

class Hall {
private:
    std::string hallId;
    std::string name;
    bool supports3D;
    int capacity;

public:
    Hall(std::string id, std::string n, bool s3d, int cap)
        : hallId(id), name(n), supports3D(s3d), capacity(cap) {}

    std::string getId() const { return hallId; }
    std::string getName() const { return name; }
    bool getSupports3D() const { return supports3D; }
    int getCapacity() const { return capacity; }
};

class Showtime {
private:
    std::string showtimeId;
    std::string movieId;
    std::string hallId;
    std::string startTime;
    double ticketPrice;

public:
    Showtime(std::string sid, std::string mid, std::string hid, std::string time, double price)
        : showtimeId(sid), movieId(mid), hallId(hid), startTime(time), ticketPrice(price) {}

    std::string getId() const { return showtimeId; }
    std::string getMovieId() const { return movieId; }
    std::string getHallId() const { return hallId; }
    std::string getStartTime() const { return startTime; }
    double getPrice() const { return ticketPrice; }
};
