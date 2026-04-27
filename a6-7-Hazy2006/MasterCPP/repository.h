#pragma once
#include <vector>
#include <string>
#include "domain.h"
#include "exceptions.h"

class Repo {
private:
    std::vector<Tutorial> tutorials;
    std::string filename;

    void loadFromFile();
    void saveToFile() const;

public:
    explicit Repo(const std::string& filename = "tutorials.txt");

    void add(const Tutorial& t);
    void remove(int index);
    void update(const Tutorial& t, int index);
    int search(const Tutorial& t) const;
    int length() const;
    const std::vector<Tutorial>& get_all() const;
};