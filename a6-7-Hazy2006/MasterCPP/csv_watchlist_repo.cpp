#include "csv_watchlist_repo.h"
#include <fstream>
#include <Windows.h>
#include <shellapi.h>

CSVWatchListRepo::CSVWatchListRepo(const std::string& filename) : filename(filename) {}

bool CSVWatchListRepo::add(const Tutorial& t) {
    if (contains(t)) return false;
    tutorials.push_back(t);
    writeToFile();
    return true;
}

void CSVWatchListRepo::remove(int index) {
    tutorials.erase(tutorials.begin() + index);
    writeToFile();
}

bool CSVWatchListRepo::contains(const Tutorial& t) const {
    return std::find(tutorials.begin(), tutorials.end(), t) != tutorials.end();
}

const std::vector<Tutorial>& CSVWatchListRepo::getAll() const {
    return tutorials;
}

int CSVWatchListRepo::length() const {
    return static_cast<int>(tutorials.size());
}

void CSVWatchListRepo::writeToFile() const {
    // Format: title,presenter,minutes,seconds,likes,link
    std::ofstream fout(filename);
    for (const auto& t : tutorials) {
        fout << t.getTitle() << ","
            << t.getPresenter() << ","
            << t.getDuration().getMinutes() << ","
            << t.getDuration().getSeconds() << ","
            << t.getLikes() << ","
            << t.getLink() << "\n";
    }
}

void CSVWatchListRepo::display() const {
    writeToFile();
    ShellExecuteA(NULL, "open", "notepad.exe", filename.c_str(), NULL, SW_SHOWMAXIMIZED);
}