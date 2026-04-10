#include <iostream>
#include <limits>
#include "ui.h"

UI::UI(Service& s) : s(s) {
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
            std::cout << "\nInvalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            std::string title, presenter, link;
            int minutes, seconds, likes;
            std::cout << "\n--- Add Tutorial ---\n";
            std::cout << "Title: "; 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, title);

            std::cout << "Presenter: "; 
            std::getline(std::cin, presenter);

            std::cout << "Minutes: "; 
            std::cin >> minutes;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nError: Invalid input for minutes!\n";
                break;
            }

            std::cout << "Seconds: "; 
            std::cin >> seconds;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nError: Invalid input for seconds!\n";
                break;
            }

            std::cout << "Likes: "; 
            std::cin >> likes;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nError: Invalid input for likes!\n";
                break;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Link: "; 
            std::getline(std::cin, link);

            try {
                s.add(title, presenter, minutes, seconds, likes, link);
                std::cout << "\nTutorial added successfully!\n";
            }
            catch (std::exception& e) {
                std::cout << "\nError: " << e.what() << "\n";
            }
            break;
        }
        case 2:
        {
            std::string title, presenter;
            std::cout << "\n--- Remove Tutorial ---\n";
            std::cout << "Title to search: "; 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, title);

            std::cout << "Presenter to search: "; 
            std::getline(std::cin, presenter);

            try {
                s.remove(title, presenter);
                std::cout << "\nTutorial deleted successfully!\n"; 
            }
            catch(std::exception& e){
                std::cout << "\nError: " << e.what() << "\n";
            }
            break;
        }
        case 3: {
            std::string title, presenter;
            std::cout << "\n--- Update Tutorial ---\n";
            std::cout << "Title to find: "; 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, title);

            std::cout << "Presenter to find: "; 
            std::getline(std::cin, presenter);
            try {
                s.find(title, presenter);
            }
            catch (std::exception& e) {
                std::cout << "\nError: " << e.what() << "\n";
                break;
            }

            std::string newTitle, newPresenter, newLink;
            int newMinutes, newSeconds;

            std::cout << "New Title: "; 
            std::getline(std::cin, newTitle);

            std::cout << "New Presenter: "; 
            std::getline(std::cin, newPresenter);

            std::cout << "New Minutes: "; 
            std::cin >> newMinutes;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nError: Invalid input for minutes!\n";
                break;
            }

            std::cout << "New Seconds: "; 
            std::cin >> newSeconds;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nError: Invalid input for seconds!\n";
                break;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "New Link: "; 
            std::getline(std::cin, newLink);

            try {
                s.update(title, presenter, newTitle, newPresenter, newMinutes, newSeconds, newLink);
                std::cout << "\nTutorial updated successfully!\n";
            }
            catch(std::exception& e){
                std::cout << "\nError: " << e.what() << "\n";  
            }
            break;
        }
        case 4: {
            DynamicVector<Tutorial> all = s.get_all();
            std::cout << "\n--- All Tutorials ---\n";
            if (all.length() == 0) {
                std::cout << "No tutorials available.\n";
            }
            else {
                for (int i = 0; i < all.length(); i++) {
                    Tutorial t = all.get(i);
                    std::cout << "\nTutorial " << (i + 1) << ":\n";
                    std::cout << "  Title:     " << t.getTitle() << "\n";
                    std::cout << "  Presenter: " << t.getPresenter() << "\n";
                    std::cout << "  Duration:  " << t.getDuration().getMinutes() << ":" 
                              << (t.getDuration().getSeconds() < 10 ? "0" : "") << t.getDuration().getSeconds() << "\n";
                    std::cout << "  Likes:     " << t.getLikes() << "\n";
                    std::cout << "  Link:      " << t.getLink() << "\n";
                }
            }
            break;
        }
        case 0:
            break;
        default:
            std::cout << "\nInvalid option! Please choose a valid menu option.\n";
            break;
        }
    } while (choice != 0);
}

void UI::printTutorial(const Tutorial& t, int index)
{
    std::cout << "\n--- Tutorial " << (index + 1) << " ---\n";
    std::cout << "  Title:     " << t.getTitle() << "\n";
    std::cout << "  Presenter: " << t.getPresenter() << "\n";
    std::cout << "  Duration:  " << t.getDuration().getMinutes() << ":"
        << (t.getDuration().getSeconds() < 10 ? "0" : "")
        << t.getDuration().getSeconds() << "\n";
    std::cout << "  Likes:     " << t.getLikes() << "\n";
}

void UI::userMenu() {
    int choice;
    do {
        std::cout << "\n--- User Menu ---\n";
        std::cout << "1. Browse tutorials & add to watch list\n";
        std::cout << "2. Delete from watch list\n";
        std::cout << "3. See watch list\n";
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
        case 1: browseAndAdd();       break;
        case 2: deleteFromWatchList(); break;
        case 3: displayWatchList();    break;
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

    DynamicVector<Tutorial> filtered = s.getByPresenter(presenter);

    if (filtered.length() == 0) {
        std::cout << "\nNo tutorials found for this presenter.\n";
        return;
    }

    int current = 0;
    int choice;

    do {
        Tutorial t = filtered.get(current);
        printTutorial(t, current);

        // Open link in browser
        std::string cmd = "start " + t.getLink();
        system(cmd.c_str());

        std::cout << "\n1. Add to watch list\n2. Next\n0. Back\n";
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
            // Check not already in watch list
            bool alreadyAdded = false;
            for (int i = 0; i < watchList.length(); i++) {
                if (watchList.get(i) == t) {
                    alreadyAdded = true;
                    break;
                }
            }
            if (alreadyAdded) {
                std::cout << "\nAlready in watch list!\n";
            }
            else {
                watchList.add(t);
                std::cout << "\nAdded to watch list!\n";
            }
            // Advance to next after adding
            current = (current + 1) % filtered.length();
            break;
        }
        case 2:
            // Circular: wrap around
            current = (current + 1) % filtered.length();
            break;
        case 0:
            break;
        default:
            std::cout << "\nInvalid option!\n";
            break;
        }
    } while (choice != 0);
}

void UI::deleteFromWatchList() {
    if (watchList.length() == 0) {
        std::cout << "\nWatch list is empty.\n";
        return;
    }

    // Show watch list with indices
    std::cout << "\n--- Your Watch List ---\n";
    for (int i = 0; i < watchList.length(); i++) {
        Tutorial t = watchList.get(i);
        std::cout << (i + 1) << ". " << t.getTitle()
            << " by " << t.getPresenter() << "\n";
    }

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
    if (idx < 1 || idx > watchList.length()) {
        std::cout << "\nInvalid index!\n";
        return;
    }

    Tutorial t = watchList.get(idx - 1);

    std::cout << "\nDid you like this tutorial? (1 = Yes, 0 = No): ";
    int liked;
    std::cin >> liked;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nInvalid input!\n";
        return;
    }

    if (liked == 1) {
        try {
            s.incrementLikes(t.getTitle(), t.getPresenter());
            std::cout << "\nLike recorded!\n";
        }
        catch (std::exception& e) {
            std::cout << "\nWarning: could not update likes — " << e.what() << "\n";
        }
    }

    watchList.remove(idx - 1);
    std::cout << "\nRemoved from watch list.\n";
}

void UI::displayWatchList() {
    if (watchList.length() == 0) {
        std::cout << "\nWatch list is empty.\n";
        return;
    }

    std::cout << "\n--- Your Watch List ---\n";
    for (int i = 0; i < watchList.length(); i++) {
        printTutorial(watchList.get(i), i);
    }
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

