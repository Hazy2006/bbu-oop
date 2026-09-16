#pragma once
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual void update() = 0;
    virtual ~Observer() = default;
};

class Subject {
private:
    std::vector<Observer*> observers;
public:
    void attach(Observer* o);
    void detach(Observer* o);
    void notify();
};
