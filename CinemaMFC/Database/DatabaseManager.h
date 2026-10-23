#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <sqlext.h>
#include "../Models/Movie.h"

class DatabaseManager {
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    bool isConnected;

    static DatabaseManager* instance;

    DatabaseManager();
    ~DatabaseManager();

public:

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager* getInstance();

    bool connect(const std::string& connectionString);
    void disconnect();
    bool getIsConnected() const { return isConnected; }

    std::vector<Movie> getAllMovies();
    bool addMovie(const Movie& movie);
};
