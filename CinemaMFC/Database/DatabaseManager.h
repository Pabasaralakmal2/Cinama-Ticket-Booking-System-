#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <sqlext.h>
#include "../Models/Movie.h"

// Singleton Class representing the Data Access Layer
class DatabaseManager {
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    bool isConnected;
    
    // Singleton instance
    static DatabaseManager* instance;

    // Private constructor (Singleton pattern)
    DatabaseManager();
    ~DatabaseManager();

public:
    // Disallow cloning
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager* getInstance();

    // Connection methods
    bool connect(const std::string& connectionString);
    void disconnect();
    bool getIsConnected() const { return isConnected; }

    // CRUD Example for Movies
    std::vector<Movie> getAllMovies();
    bool addMovie(const Movie& movie);
};
