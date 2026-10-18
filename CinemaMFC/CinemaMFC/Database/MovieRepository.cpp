#include "pch.h"
#include "MovieRepository.h"
#include "vendor/json.hpp"
#include <iostream>
#include <sstream>
using json = nlohmann::json;
static std::string jStr(const json& obj, const std::string& key) {
    if (obj.contains(key) && obj[key].is_string()) return obj[key].get<std::string>();
    return "";
}
static int jInt(const json& obj, const std::string& key) {
    if (obj.contains(key) && obj[key].is_number()) return obj[key].get<int>();
    return 0;
}
static std::vector<Movie> parseMovies(const std::string& jsonStr) {
    std::vector<Movie> movies;
    if (jsonStr.empty()) return movies;
    try {
        auto arr = json::parse(jsonStr);
        for (auto& obj : arr) {
            Movie m;
            m.SetMovieId   (jInt(obj, "movie_id"));
            m.SetTitle     (jStr(obj, "title"));
            m.SetGenre     (jStr(obj, "genre"));
            m.SetDuration  (jStr(obj, "duration"));
            m.SetDescription(jStr(obj, "description"));
            m.SetPosterUrl (jStr(obj, "poster_url"));
            m.SetTrailerUrl(jStr(obj, "trailer_url"));
            m.SetFormat    (jStr(obj, "format"));
            m.SetReleaseDate(jStr(obj, "release_date"));
            m.SetStatus    (jStr(obj, "status"));
            movies.push_back(m);
        }
    } catch (...) {
        std::cerr << "MovieRepository: JSON parse error\n";
    }
    return movies;
}
bool MovieRepository::AddMovie(const Movie& movie) {
    json body;
    body["title"]        = movie.GetTitle();
    body["genre"]        = movie.GetGenre();
    body["duration"]     = movie.GetDuration();
    if (!movie.GetDescription().empty()) body["description"]  = movie.GetDescription();
    if (!movie.GetPosterUrl().empty())   body["poster_url"]   = movie.GetPosterUrl();
    if (!movie.GetTrailerUrl().empty())  body["trailer_url"]  = movie.GetTrailerUrl();
    body["format"]       = movie.GetFormat();
    if (!movie.GetReleaseDate().empty()) body["release_date"] = movie.GetReleaseDate();
    body["status"]       = movie.GetStatus();
    std::string resp = DatabaseManager::GetInstance().HttpPost("movies", body.dump());
    return !resp.empty();
}
std::vector<Movie> MovieRepository::GetAllMovies() {
    std::string resp = DatabaseManager::GetInstance().HttpGet(
        "movies",
        "select=movie_id,title,genre,duration,description,poster_url,trailer_url,format,release_date,status"
    );
    return parseMovies(resp);
}
std::unique_ptr<Movie> MovieRepository::GetMovieById(int id) {
    std::string query = "select=movie_id,title,genre,duration,description,poster_url,trailer_url,format,release_date,status&movie_id=eq." + std::to_string(id);
    std::string resp  = DatabaseManager::GetInstance().HttpGet("movies", query);
    auto movies = parseMovies(resp);
    if (movies.empty()) return nullptr;
    return std::make_unique<Movie>(movies[0]);
}
bool MovieRepository::UpdateMovie(const Movie& movie) {
    json body;
    body["title"]        = movie.GetTitle();
    body["genre"]        = movie.GetGenre();
    body["duration"]     = movie.GetDuration();
    if (!movie.GetDescription().empty()) body["description"]  = movie.GetDescription();
    if (!movie.GetPosterUrl().empty())   body["poster_url"]   = movie.GetPosterUrl();
    if (!movie.GetTrailerUrl().empty())  body["trailer_url"]  = movie.GetTrailerUrl();
    body["format"]       = movie.GetFormat();
    if (!movie.GetReleaseDate().empty()) body["release_date"] = movie.GetReleaseDate();
    body["status"]       = movie.GetStatus();
    std::string query = "movie_id=eq." + std::to_string(movie.GetMovieId());
    std::string resp  = DatabaseManager::GetInstance().HttpPatch("movies", query, body.dump());
    return !resp.empty();
}
bool MovieRepository::DeleteMovie(int id) {
    return DatabaseManager::GetInstance().HttpDelete("movies", "movie_id=eq." + std::to_string(id));
}
