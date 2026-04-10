#pragma once
#include "service.h"

class UI {
private:
    Service& s;
    DynamicVector<Tutorial> watchList;

    void printTutorial(const Tutorial& t, int index);
    void adminMenu();
    void userMenu();
    void browseAndAdd();
    void deleteFromWatchList();
    void displayWatchList();

public:
    UI(Service& s);
    void run();
};