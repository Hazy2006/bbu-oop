#include "html_watchlist_repo.h"
#include <fstream>
#include <Windows.h>
#include <shellapi.h>

HTMLWatchListRepo::HTMLWatchListRepo(const std::string& filename) : filename(filename) {}

bool HTMLWatchListRepo::add(const Tutorial& t) {
    if (contains(t)) return false;
    tutorials.push_back(t);
    writeToFile();
    return true;
}

void HTMLWatchListRepo::remove(int index) {
    tutorials.erase(tutorials.begin() + index);
    writeToFile();
}

bool HTMLWatchListRepo::contains(const Tutorial& t) const {
    return std::find(tutorials.begin(), tutorials.end(), t) != tutorials.end();
}

const std::vector<Tutorial>& HTMLWatchListRepo::getAll() const {
    return tutorials;
}

int HTMLWatchListRepo::length() const {
    return static_cast<int>(tutorials.size());
}

void HTMLWatchListRepo::writeToFile() const {
    std::ofstream fout(filename);
    fout << "<!DOCTYPE html>\n<html>\n<head>\n";
    fout << "<title>Watch List</title>\n";
    fout << "<style>\n";
    fout << "body { font-family: Arial, sans-serif; margin: 30px; }\n";
    fout << "table { border-collapse: collapse; width: 100%; }\n";
    fout << "th { background-color: #4a90d9; color: white; padding: 10px; text-align: left; }\n";
    fout << "td { padding: 8px 10px; border-bottom: 1px solid #ddd; }\n";
    fout << "tr:hover { background-color: #f1f1f1; }\n";
    fout << "a { color: #4a90d9; }\n";
    fout << "</style>\n</head>\n<body>\n";
    fout << "<h2>My Tutorial Watch List</h2>\n";
    fout << "<table>\n";
    fout << "<tr><th>Title</th><th>Presenter</th><th>Duration</th><th>Likes</th><th>Link</th></tr>\n";
    for (const auto& t : tutorials) {
        fout << "<tr>";
        fout << "<td>" << t.getTitle() << "</td>";
        fout << "<td>" << t.getPresenter() << "</td>";
        fout << "<td>" << t.getDuration().getMinutes() << ":"
            << (t.getDuration().getSeconds() < 10 ? "0" : "")
            << t.getDuration().getSeconds() << "</td>";
        fout << "<td>" << t.getLikes() << "</td>";
        fout << "<td><a href=\"" << t.getLink() << "\">" << t.getLink() << "</a></td>";
        fout << "</tr>\n";
    }
    fout << "</table>\n</body>\n</html>\n";
}

void HTMLWatchListRepo::display() const {
    writeToFile();
    // ShellExecuteA with NULL verb opens the file with its default associated application
    // (the default browser for .html files)
    ShellExecuteA(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}