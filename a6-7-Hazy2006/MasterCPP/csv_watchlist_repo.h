#pragma once
#include "watchlist_repo.h"
#include <string>
#include <algorithm>

// Stores the watch list as a pipe-delimited CSV file.
// Displays it by opening with Notepad.
class CSVWatchListRepo : public WatchListRepo {
private:
    std::vector<Tutorial> tutorials;
    std::string filename;

public:
    explicit CSVWatchListRepo(const std::string& filename);

    bool add(const Tutorial& t) override;
    void remove(int index) override;
    bool contains(const Tutorial& t) const override;
    const std::vector<Tutorial>& getAll() const override;
    int length() const override;

    void writeToFile() const override;
    void display() const override;
};