#pragma once
#include "Movie.h"
#include "DatabaseManager.h"
#include <vector>
#include <memory>
class MovieRepository {
public:
    MovieRepository() {}
    ~MovieRepository() {}
    bool AddMovie(const Movie& movie);                 
    std::vector<Movie> GetAllMovies();                 
    std::unique_ptr<Movie> GetMovieById(int id);       
    bool UpdateMovie(const Movie& movie);              
    bool DeleteMovie(int id);                          
};
