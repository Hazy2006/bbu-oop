#include "sqlite_watchlist_repo.h"
#include <stdexcept>
#include <iostream>
#include <Windows.h>
#include <shellapi.h>

SQLiteWatchListRepo::SQLiteWatchListRepo(const std::string& dbFilename)
    : dbFilename(dbFilename), db(nullptr) {
    if (sqlite3_open(dbFilename.c_str(), &db) != SQLITE_OK) {
        throw RepositoryException("Cannot open SQLite database: " + std::string(sqlite3_errmsg(db)));
    }
    initDB();
    loadFromDB();
}

SQLiteWatchListRepo::~SQLiteWatchListRepo() {
    if (db) sqlite3_close(db);
}

void SQLiteWatchListRepo::initDB() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS watchlist ("
        "title TEXT NOT NULL,"
        "presenter TEXT NOT NULL,"
        "minutes INTEGER NOT NULL,"
        "seconds INTEGER NOT NULL,"
        "likes INTEGER NOT NULL,"
        "link TEXT NOT NULL,"
        "PRIMARY KEY (title, presenter)"
        ");";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string err(errMsg);
        sqlite3_free(errMsg);
        throw RepositoryException("Failed to initialise SQLite table: " + err);
    }
}

void SQLiteWatchListRepo::loadFromDB() {
    cache.clear();
    const char* sql = "SELECT title, presenter, minutes, seconds, likes, link FROM watchlist;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string presenter = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int minutes = sqlite3_column_int(stmt, 2);
        int seconds = sqlite3_column_int(stmt, 3);
        int likes = sqlite3_column_int(stmt, 4);
        std::string link = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        cache.emplace_back(title, presenter, minutes, seconds, likes, link);
    }
    sqlite3_finalize(stmt);
}

void SQLiteWatchListRepo::insertToDB(const Tutorial& t) const {
    const char* sql =
        "INSERT INTO watchlist (title, presenter, minutes, seconds, likes, link) "
        "VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, t.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, t.getPresenter().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, t.getDuration().getMinutes());
    sqlite3_bind_int(stmt, 4, t.getDuration().getSeconds());
    sqlite3_bind_int(stmt, 5, t.getLikes());
    sqlite3_bind_text(stmt, 6, t.getLink().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void SQLiteWatchListRepo::deleteFromDB(const Tutorial& t) const {
    const char* sql = "DELETE FROM watchlist WHERE title = ? AND presenter = ?;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, t.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, t.getPresenter().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

bool SQLiteWatchListRepo::add(const Tutorial& t) {
    if (contains(t)) return false;
    insertToDB(t);
    cache.push_back(t);
    return true;
}

void SQLiteWatchListRepo::remove(int index) {
    Tutorial t = cache[index];
    deleteFromDB(t);
    cache.erase(cache.begin() + index);
}

bool SQLiteWatchListRepo::contains(const Tutorial& t) const {
    return std::find(cache.begin(), cache.end(), t) != cache.end();
}

const std::vector<Tutorial>& SQLiteWatchListRepo::getAll() const {
    return cache;
}

int SQLiteWatchListRepo::length() const {
    return static_cast<int>(cache.size());
}

void SQLiteWatchListRepo::writeToFile() const {
}

void SQLiteWatchListRepo::display() const {
    // Falls back to printing contents to console if not installed
    int result = (int)(intptr_t)ShellExecuteA(
        NULL, "open", dbFilename.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);

    if (result <= 32) {
        // ShellExecute failed — print to console instead
        std::cout << "\n--- Watch List (SQLite) ---\n";
        for (int i = 0; i < (int)cache.size(); i++) {
            const Tutorial& t = cache[i];
            std::cout << (i + 1) << ". " << t.getTitle()
                << " by " << t.getPresenter()
                << " (" << t.getDuration().getMinutes() << ":"
                << (t.getDuration().getSeconds() < 10 ? "0" : "")
                << t.getDuration().getSeconds() << ")"
                << " | Likes: " << t.getLikes()
                << "\n   " << t.getLink() << "\n";
        }
    }
}