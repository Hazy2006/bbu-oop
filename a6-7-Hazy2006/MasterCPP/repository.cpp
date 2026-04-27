#include "repository.h"
#include "exceptions.h"
#include <fstream>
#include <algorithm>

Repo::Repo(const std::string& filename) : filename(filename) {
    loadFromFile();
}

void Repo::loadFromFile() {
    std::ifstream fin(filename);
    if (!fin.is_open()) return;
    Tutorial t;
    while (fin >> t)
        if (!t.getTitle().empty())
            tutorials.push_back(t);
}

void Repo::saveToFile() const {
    std::ofstream fout(filename);
    for (const auto& t : tutorials)
        fout << t << "\n";
}

void Repo::add(const Tutorial& t) {
    if (std::find(tutorials.begin(), tutorials.end(), t) != tutorials.end())
        throw RepositoryException("Tutorial already exists!");
    tutorials.push_back(t);
    saveToFile();
}

void Repo::remove(int index) {
    tutorials.erase(tutorials.begin() + index);
    saveToFile();
}

void Repo::update(const Tutorial& t, int index) {
    tutorials[index] = t;
    saveToFile();
}

int Repo::search(const Tutorial& t) const {
    auto it = std::find(tutorials.begin(), tutorials.end(), t);
    if (it == tutorials.end()) return -1;
    return static_cast<int>(it - tutorials.begin());
}

int Repo::length() const {
    return static_cast<int>(tutorials.size());
}

const std::vector<Tutorial>& Repo::get_all() const {
    return tutorials;
}