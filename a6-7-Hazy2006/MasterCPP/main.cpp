#include <iostream>
#include <string>
#include "ui.h"
#include "repository.h"
#include "service.h"
#include "watchlist_service.h"
#include "csv_watchlist_repo.h"
#include "html_watchlist_repo.h"
#include "tests.h"

int main() {
    runAllTests();

    // Ask the user which file format to use for the watch list
    std::string fileType;
    std::cout << "Choose watch list file type (csv / html): ";
    std::cin >> fileType;

    // Instantiate the correct concrete repo — polymorphism kicks in here
    WatchListRepo* watchListRepo = nullptr;
    if (fileType == "csv")
        watchListRepo = new CSVWatchListRepo("watchlist.csv");
    else
        watchListRepo = new HTMLWatchListRepo("watchlist.html");

    Repo repo("tutorials.txt");
    Service service(repo);
    WatchListService watchListService(*watchListRepo);
    UI ui(service, watchListService);
    ui.run();

    delete watchListRepo;
    return 0;
}