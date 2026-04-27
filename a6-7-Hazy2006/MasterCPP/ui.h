#pragma once
#include <vector>
#include "service.h"
#include "watchlist_service.h"

class UI {
private:
    Service& s;
    WatchListService& watchListService;

    void printTutorial(const Tutorial& t, int index);
    void adminMenu();
    void userMenu();
    void browseAndAdd();
    void deleteFromWatchList();
    void displayWatchList();

public:
    UI(Service& s, WatchListService& wls);
    void run();
};