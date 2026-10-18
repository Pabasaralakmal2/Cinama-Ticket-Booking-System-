#pragma once
#include <string>
class Movie {
private:
    int movieId;
    std::string title;
    std::string genre;
    std::string duration;
    std::string description;
    std::string posterUrl;
    std::string trailerUrl;
    std::string format;
    std::string releaseDate;
    std::string status;
public:
    Movie() : movieId(0) {}
    int GetMovieId() const { return movieId; }
    std::string GetTitle() const { return title; }
    std::string GetGenre() const { return genre; }
    std::string GetDuration() const { return duration; }
    std::string GetDescription() const { return description; }
    std::string GetPosterUrl() const { return posterUrl; }
    std::string GetTrailerUrl() const { return trailerUrl; }
    std::string GetFormat() const { return format; }
    std::string GetReleaseDate() const { return releaseDate; }
    std::string GetStatus() const { return status; }
    void SetMovieId(int id) { movieId = id; }
    void SetTitle(const std::string& t) { title = t; }
    void SetGenre(const std::string& g) { genre = g; }
    void SetDuration(const std::string& d) { duration = d; }
    void SetDescription(const std::string& desc) { description = desc; }
    void SetPosterUrl(const std::string& url) { posterUrl = url; }
    void SetTrailerUrl(const std::string& url) { trailerUrl = url; }
    void SetFormat(const std::string& f) { format = f; }
    void SetReleaseDate(const std::string& rd) { releaseDate = rd; }
    void SetStatus(const std::string& s) { status = s; }
};
