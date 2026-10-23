#include "DatabaseManager.h"
#include <iostream>

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() : hEnv(SQL_NULL_HENV), hDbc(SQL_NULL_HDBC), isConnected(false) {

    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_SUCCESS) {
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    }
}

DatabaseManager::~DatabaseManager() {
    disconnect();
    if (hEnv != SQL_NULL_HENV) {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }
}

DatabaseManager* DatabaseManager::getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseManager();
    }
    return instance;
}

bool DatabaseManager::connect(const std::string& connectionString) {
    if (isConnected) return true;

    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) == SQL_SUCCESS) {
        SQLCHAR retConString[1024];
        SQLSMALLINT retLength;

        SQLRETURN ret = SQLDriverConnectA(hDbc, NULL,
            (SQLCHAR*)connectionString.c_str(), SQL_NTS,
            retConString, 1024, &retLength, SQL_DRIVER_NOPROMPT);

        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            isConnected = true;
            return true;
        } else {
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
            hDbc = SQL_NULL_HDBC;
        }
    }
    return false;
}

void DatabaseManager::disconnect() {
    if (isConnected && hDbc != SQL_NULL_HDBC) {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        hDbc = SQL_NULL_HDBC;
        isConnected = false;
    }
}

std::vector<Movie> DatabaseManager::getAllMovies() {
    std::vector<Movie> movies;
    if (!isConnected) return movies;

    SQLHSTMT hStmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS) {
        std::string query = "SELECT movie_id, title, genre, duration_mins, format, status FROM Movies";
        if (SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS) == SQL_SUCCESS) {

            char id[40], title[100], genre[50], format[20], status[20];
            int duration;

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_CHAR, id, sizeof(id), NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, title, sizeof(title), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, genre, sizeof(genre), NULL);
                SQLGetData(hStmt, 4, SQL_C_LONG, &duration, 0, NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, format, sizeof(format), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, status, sizeof(status), NULL);

                movies.push_back(Movie(id, title, genre, duration, format, status));
            }
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    return movies;
}

bool DatabaseManager::addMovie(const Movie& movie) {
    if (!isConnected) return false;
    bool success = false;
    SQLHSTMT hStmt;

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS) {

        std::string query = "INSERT INTO Movies (title, genre, duration_mins, format, status) VALUES ('" +
            movie.getTitle() + "', '" + movie.getGenre() + "', " + std::to_string(movie.getDuration()) +
            ", '" + movie.getFormat() + "', '" + movie.getStatus() + "')";

        if (SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS) == SQL_SUCCESS) {
            success = true;
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    return success;
}
