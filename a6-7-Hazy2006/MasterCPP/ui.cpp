#include <iostream>
#include <limits>
#include "ui.h"

UI::UI(Service& s, WatchListService& wls) : s(s), watchListService(wls) {}

void UI::printTutorial(const Tutorial& t, int index) {
    std::cout << "\n--- Tutorial " << (index + 1) << " ---\n";
    std::cout << "  Title:     " << t.getTitle() << "\n";
    std::cout << "  Presenter: " << t.getPresenter() << "\n";
    std::cout << "  Duration:  " << t.getDuration().getMinutes() << ":"
        << (t.getDuration().getSeconds() < 10 ? "0" : "")
        << t.getDuration().getSeconds() << "\n";
    std::cout << "  Likes:     " << t.getLikes() << "\n";
}

void UI::adminMenu() {
    int choice;
    do {
        std::cout << "\n--- Admin Menu ---\n";
        std::cout << "1. Add\n2. Remove\n3. Update\n4. Display all\n0. Back\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input!\n";
            continue;
        }
        switch (choice) {
        case 1: {
            std::string title, presenter, link;
            int minutes, seconds, likes;
            std::cout << "\n--- Add Tutorial ---\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Title: ";     std::getline(std::cin, title);
            std::cout << "Presenter: "; std::getline(std::cin, presenter);
            std::cout << "Minutes: ";   std::cin >> minutes;
            std::cout << "Seconds: ";   std::cin >> seconds;
            std::cout << "Likes: ";     std::cin >> likes;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Link: ";      std::getline(std::cin, link);
            try {
                s.add(title, presenter, minutes, seconds, likes, link);
                std::cout << "\nTutorial added successfully!\n";
            }
            catch (std::exception& e) { std::cout << "\nError: " << e.what() << "\n"; }
            break;
        }
        case 2: {
            std::string title, presenter;
            std::cout << "\n--- Remove Tutorial ---\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Title: ";     std::getline(std::cin, title);
            std::cout << "Presenter: "; std::getline(std::cin, presenter);
            try {
                s.remove(title, presenter);
                std::cout << "\nTutorial deleted successfully!\n";
            }
            catch (std::exception& e) { std::cout << "\nError: " << e.what() << "\n"; }
            break;
        }
        case 3: {
            std::string title, presenter;
            std::cout << "\n--- Update Tutorial ---\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Title to find: ";     std::getline(std::cin, title);
            std::cout << "Presenter to find: "; std::getline(std::cin, presenter);
            try { s.find(title, presenter); }
            catch (std::exception& e) { std::cout << "\nError: " << e.what() << "\n"; break; }
            std::string newTitle, newPresenter, newLink;
            int newMinutes, newSeconds;
            std::cout << "New Title: ";     std::getline(std::cin, newTitle);
            std::cout << "New Presenter: "; std::getline(std::cin, newPresenter);
            std::cout << "New Minutes: ";   std::cin >> newMinutes;
            std::cout << "New Seconds: ";   std::cin >> newSeconds;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "New Link: ";      std::getline(std::cin, newLink);
            try {
                s.update(title, presenter, newTitle, newPresenter, newMinutes, newSeconds, newLink);
                std::cout << "\nTutorial updated successfully!\n";
            }
            catch (std::exception& e) { std::cout << "\nError: " << e.what() << "\n"; }
            break;
        }
        case 4: {
            std::vector<Tutorial> all = s.get_all();
            std::cout << "\n--- All Tutorials ---\n";
            if (all.empty()) {
                std::cout << "No tutorials available.\n";
            }
            else {
                int i = 0;
                for (const auto& t : all) {
                    printTutorial(t, i);
                    std::cout << "  Link: " << t.getLink() << "\n";
                    i++;
                }
            }
            break;
        }
        case 0: break;
        default: std::cout << "\nInvalid option!\n"; break;
        }
    } while (choice != 0);
}

void UI::userMenu() {
    int choice;
    do {
        std::cout << "\n--- User Menu ---\n";
        std::cout << "1. Browse tutorials & add to watch list\n";
        std::cout << "2. Delete from watch list\n";
        std::cout << "3. See watch list\n";
        std::cout << "4. Open watch list file\n";  // Req 5
        std::cout << "0. Back\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input!\n";
            continue;
        }
        switch (choice) {
        case 1: browseAndAdd();        break;
        case 2: deleteFromWatchList(); break;
        case 3: displayWatchList();    break;
        case 4:
            watchListService.displayFile();  // opens CSV in Notepad or HTML in browser
            break;
        case 0: break;
        default: std::cout << "\nInvalid option!\n"; break;
        }
    } while (choice != 0);
}

void UI::browseAndAdd() {
    std::string presenter;
    std::cout << "\nEnter presenter name (leave empty for all): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, presenter);

    std::vector<Tutorial> filtered = s.getByPresenter(presenter);
    if (filtered.empty()) {
        std::cout << "\nNo tutorials found for this presenter.\n";
        return;
    }

    int current = 0;
    int choice;
    do {
        Tutorial t = filtered[current];
        printTutorial(t, current);
        system(("start " + t.getLink()).c_str());

        std::cout << "\n1. Add to watch list\n2. Next\n0. Back\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch (choice) {
        case 1:
            if (watchListService.add(t))
                std::cout << "\nAdded to watch list!\n";
            else
                std::cout << "\nAlready in watch list!\n";
            current = (current + 1) % (int)filtered.size();
            break;
        case 2:
            current = (current + 1) % (int)filtered.size();
            break;
        case 0: break;
        default: std::cout << "\nInvalid option!\n"; break;
        }
    } while (choice != 0);
}

void UI::deleteFromWatchList() {
    if (watchListService.length() == 0) {
        std::cout << "\nWatch list is empty.\n";
        return;
    }
    std::cout << "\n--- Your Watch List ---\n";
    for (int i = 0; i < watchListService.length(); i++)
        std::cout << (i + 1) << ". " << watchListService.getAll()[i].getTitle()
        << " by " << watchListService.getAll()[i].getPresenter() << "\n";

    std::cout << "\nEnter number of tutorial to delete (0 to cancel): ";
    int idx;
    std::cin >> idx;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nInvalid input!\n";
        return;
    }
    if (idx == 0) return;
    if (idx < 1 || idx > watchListService.length()) {
        std::cout << "\nInvalid index!\n";
        return;
    }

    Tutorial t = watchListService.getAll()[idx - 1];

    std::cout << "\nDid you like this tutorial? (1 = Yes, 0 = No): ";
    int liked;
    std::cin >> liked;
    if (!std::cin.fail() && liked == 1) {
        try {
            s.incrementLikes(t.getTitle(), t.getPresenter());
            std::cout << "\nLike recorded!\n";
        }
        catch (std::exception& e) {
            std::cout << "\nWarning: " << e.what() << "\n";
        }
    }
    watchListService.remove(idx - 1);
    std::cout << "\nRemoved from watch list.\n";
}

void UI::displayWatchList() {
    if (watchListService.length() == 0) {
        std::cout << "\nWatch list is empty.\n";
        return;
    }
    std::cout << "\n--- Your Watch List ---\n";
    for (int i = 0; i < watchListService.length(); i++)
        printTutorial(watchListService.getAll()[i], i);
}

void UI::run() {
    int choice;
    do {
        std::cout << "\n=== MasterCPP Tutorial Manager ===\n";
        std::cout << "1. Admin mode\n2. User mode\n0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input!\n";
            continue;
        }
        switch (choice) {
        case 1: adminMenu(); break;
        case 2: userMenu();  break;
        case 0: std::cout << "\nGoodbye!\n"; break;
        default: std::cout << "\nInvalid option!\n"; break;
        }
    } while (choice != 0);
}