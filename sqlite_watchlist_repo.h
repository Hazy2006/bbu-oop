#pragma once
#include "watchlist_repo.h"
#include "sqlite3.h"
#include <string>
#include <algorithm>
#include "exceptions.h"

// Database-backed watch list repository using SQLite.
// Inherits from WatchListRepo
// The database file is created automatically on first run.
class SQLiteWatchListRepo : public WatchListRepo {
private:
    std::vector<Tutorial> cache;  // in-memory cache, kept in sync with DB
    std::string dbFilename;
    sqlite3* db;

    void initDB();           // create table if not exists
    void loadFromDB();       // populate cache from DB on startup
    void insertToDB(const Tutorial& t) const;
    void deleteFromDB(const Tutorial& t) const;

public:
    explicit SQLiteWatchListRepo(const std::string& dbFilename);
    ~SQLiteWatchListRepo();

    bool add(const Tutorial& t) override;
    void remove(int index) override;
    bool contains(const Tutorial& t) const override;
    const std::vector<Tutorial>& getAll() const override;
    int length() const override;

    void writeToFile() const override;

    // Opens the .db file with DB Browser for SQLite if available,
    // otherwise falls back to printing the contents to console
    void display() const override;
};