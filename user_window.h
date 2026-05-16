#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
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

    QLabel*      indexLabel;
    QLabel*      titleVal;
    QLabel*      presenterVal;
    QLabel*      durationVal;
    QLabel*      likesVal;
    QLabel*      linkVal;
    QPushButton* prevBtn;
    QPushButton* nextBtn;
    QPushButton* addToWatchlistBtn;
    QPushButton* openFileBtn;
    QLabel*      statusLabel;

    void setupUI();
    void showCurrentTutorial();

private slots:
    void onPrev();
    void onNext();
    void onAddToWatchlist();
    void onOpenFile();
};
