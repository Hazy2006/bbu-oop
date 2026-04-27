#pragma once
#include "watchlist_repo.h"

// WatchListService delegates all storage to a WatchListRepo.
// The concrete repo type (CSV or HTML) is decided at startup and injected here.
class WatchListService {
private:
    WatchListRepo& repo;

public:
    explicit WatchListService(WatchListRepo& repo);

    bool add(const Tutorial& t);
    void remove(int index);
    bool contains(const Tutorial& t) const;
    const std::vector<Tutorial>& getAll() const;
    int length() const;
    int countByPresenter(const std::string& presenter) const;

    // Persist and open the watch list file.
    void displayFile() const;
};