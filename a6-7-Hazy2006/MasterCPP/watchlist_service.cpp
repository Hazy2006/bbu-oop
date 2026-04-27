#include "watchlist_service.h"
#include <algorithm>

WatchListService::WatchListService(WatchListRepo& repo) : repo(repo) {}

bool WatchListService::add(const Tutorial& t) {
    return repo.add(t);
}

void WatchListService::remove(int index) {
    repo.remove(index);
}

bool WatchListService::contains(const Tutorial& t) const {
    return repo.contains(t);
}

const std::vector<Tutorial>& WatchListService::getAll() const {
    return repo.getAll();
}

int WatchListService::length() const {
    return repo.length();
}

int WatchListService::countByPresenter(const std::string& presenter) const {
    const auto& all = repo.getAll();
    return static_cast<int>(std::count_if(all.begin(), all.end(),
        [&presenter](const Tutorial& t) { return t.getPresenter() == presenter; }));
}

void WatchListService::displayFile() const {
    repo.display();
}