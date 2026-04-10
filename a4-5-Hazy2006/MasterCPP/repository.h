#pragma once
#include "domain.h"
#include "vector.h"

class Repo {
private:
    DynamicVector<Tutorial> v;
public:
    void add(const Tutorial& t);
    int length() const;
    void remove(int index);
    void update(Tutorial element, int index);
    DynamicVector<Tutorial> get_all() const;
    int search(const Tutorial& t);
};