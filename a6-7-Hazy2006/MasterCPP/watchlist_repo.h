#pragma once
#include <vector>
#include "domain.h"

// Abstract base class for the user's watch list repository.
// Concrete subclasses handle CSV and HTML persistence.
class WatchListRepo {
public:
    virtual bool add(const Tutorial& t) = 0;
    virtual void remove(int index) = 0;
    virtual bool contains(const Tutorial& t) const = 0;
    virtual const std::vector<Tutorial>& getAll() const = 0;
    virtual int length() const = 0;

    // Persist the current watch list to the file.
    virtual void writeToFile() const = 0;

    // Open the file with the appropriate application.
    virtual void display() const = 0;

    virtual ~WatchListRepo() = default;
};