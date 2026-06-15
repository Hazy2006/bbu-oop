#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <vector>
#include "service.h"
#include "watchlist_service.h"

class UserWindow : public QWidget {
    Q_OBJECT

public:
    explicit UserWindow(Service& service, WatchListService& watchListService,
                        QWidget* parent = nullptr);

private:
    Service&          service;
    WatchListService& watchListService;
    int               currentIndex;
    std::vector<Tutorial> filteredTutorials;

    // Filter bar
    QLineEdit*   filterEdit;
    QPushButton* filterBtn;
    QPushButton* showAllBtn;

    // Tutorial display
    QLabel*      indexLabel;
    QLabel*      titleVal;
    QLabel*      presenterVal;
    QLabel*      durationVal;
    QLabel*      likesVal;
    QLabel*      linkVal;

    // Navigation
    QPushButton* prevBtn;
    QPushButton* nextBtn;

    // Actions
    QPushButton* addToWatchlistBtn;
    QPushButton* removeFromWatchlistBtn;
    QPushButton* openFileBtn;
    QPushButton* viewWatchlistBtn;

    QLabel*      statusLabel;

    // Permanent watchlist table
    QTableWidget* watchlistTable;

    void setupUI();
    void showCurrentTutorial();
    void refreshWatchlistTable();

private slots:
    void onFilter();
    void onShowAll();
    void onPrev();
    void onNext();
    void onAddToWatchlist();
    void onRemoveFromWatchlist();
    void onOpenFile();
    void onViewWatchlist();
};
