#include <iostream>
#include <string>

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "admin_window.h"
#include "user_window.h"
#include "repository.h"
#include "service.h"
#include "watchlist_service.h"
#include "csv_watchlist_repo.h"
#include "html_watchlist_repo.h"
#include "sqlite_watchlist_repo.h"
#include "ui.h"
#include "tests.h"
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

// Original console mode — pass --console on the command line to use it.
static int runConsole() {
    std::string fileType;
    std::cout << "Choose watch list file type (csv / html / sqlite): ";
    std::cin >> fileType;

    WatchListRepo* watchListRepo = nullptr;
    if (fileType == "csv")
        watchListRepo = new CSVWatchListRepo("watchlist.csv");
    else if (fileType == "sqlite")
        watchListRepo = new SQLiteWatchListRepo("watchlist.db");
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

int main(int argc, char* argv[]) {
    runAllTests();

    // --console flag -> original console UI, no Qt
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--console")
            return runConsole();
    }

    // Qt GUI mode
    QApplication app(argc, argv);

    Repo repo("tutorials.txt");
    Service service(repo);
    CSVWatchListRepo csvRepo("watchlist.csv");
    HTMLWatchListRepo htmlRepo("watchlist.html");
    WatchListService watchListService(csvRepo, &htmlRepo);

    // Mode selection dialog
    QDialog modeDialog;
    modeDialog.setWindowTitle("MasterCPP");
    modeDialog.setFixedSize(250, 130);
    QVBoxLayout* layout = new QVBoxLayout(&modeDialog);
    layout->addWidget(new QLabel("Select application mode:"));
    QPushButton* adminBtn = new QPushButton("Admin");
    QPushButton* userBtn  = new QPushButton("User");
    layout->addWidget(adminBtn);
    layout->addWidget(userBtn);

    int mode = 0;
    QObject::connect(adminBtn, &QPushButton::clicked, [&] { mode = 1; modeDialog.accept(); });
    QObject::connect(userBtn,  &QPushButton::clicked, [&] { mode = 2; modeDialog.accept(); });
    modeDialog.exec();

    if (mode == 0) return 0; // dialog was closed without a selection

    if (mode == 1) {
        AdminWindow win(service);
        win.show();
        return app.exec();
    } else {
        UserWindow win(service, watchListService);
        win.show();
        return app.exec();
    }
}
