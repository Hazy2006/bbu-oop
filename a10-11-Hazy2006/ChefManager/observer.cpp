#include "observer.h"

void Subject::attach(Observer* o) {
    observers.push_back(o);
}

void Subject::detach(Observer* o) {
    observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
}

void Subject::notify() {
    for (auto obs : observers) {
        obs->update();
    }
}
