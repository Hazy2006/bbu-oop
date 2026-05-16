#include "user_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>

UserWindow::UserWindow(Service& service, WatchListService& watchListService, QWidget* parent)
    : QWidget(parent), service(service), watchListService(watchListService), currentIndex(0)
{
    setupUI();
    showCurrentTutorial();
}

void UserWindow::setupUI() {
    setWindowTitle("MasterCPP \xe2\x80\x93 User");
    resize(500, 380);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Position indicator
    indexLabel = new QLabel;
    indexLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(indexLabel);

    // Tutorial info
    QGroupBox*   infoBox = new QGroupBox("Current Tutorial");
    QGridLayout* grid    = new QGridLayout(infoBox);

    titleVal     = new QLabel; titleVal->setWordWrap(true);
    presenterVal = new QLabel;
    durationVal  = new QLabel;
    likesVal     = new QLabel;
    linkVal      = new QLabel; linkVal->setWordWrap(true);

    grid->addWidget(new QLabel("Title:"),     0, 0); grid->addWidget(titleVal,     0, 1);
    grid->addWidget(new QLabel("Presenter:"), 1, 0); grid->addWidget(presenterVal, 1, 1);
    grid->addWidget(new QLabel("Duration:"),  2, 0); grid->addWidget(durationVal,  2, 1);
    grid->addWidget(new QLabel("Likes:"),     3, 0); grid->addWidget(likesVal,     3, 1);
    grid->addWidget(new QLabel("Link:"),      4, 0); grid->addWidget(linkVal,      4, 1);
    grid->setColumnStretch(1, 1);
    mainLayout->addWidget(infoBox);

    // Navigation
    QHBoxLayout* navLayout = new QHBoxLayout;
    prevBtn = new QPushButton("<- Previous");
    nextBtn = new QPushButton("Next ->");
    navLayout->addWidget(prevBtn);
    navLayout->addWidget(nextBtn);
    mainLayout->addLayout(navLayout);

    // Actions
    QHBoxLayout* actLayout = new QHBoxLayout;
    addToWatchlistBtn = new QPushButton("Add to Watchlist");
    openFileBtn       = new QPushButton("Open Watchlist File");
    actLayout->addWidget(addToWatchlistBtn);
    actLayout->addWidget(openFileBtn);
    mainLayout->addLayout(actLayout);

    // Status
    statusLabel = new QLabel;
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    connect(prevBtn,           &QPushButton::clicked, this, &UserWindow::onPrev);
    connect(nextBtn,           &QPushButton::clicked, this, &UserWindow::onNext);
    connect(addToWatchlistBtn, &QPushButton::clicked, this, &UserWindow::onAddToWatchlist);
    connect(openFileBtn,       &QPushButton::clicked, this, &UserWindow::onOpenFile);
}

void UserWindow::showCurrentTutorial() {
    auto all = service.get_all();
    bool empty = all.empty();

    prevBtn->setEnabled(!empty);
    nextBtn->setEnabled(!empty);
    addToWatchlistBtn->setEnabled(!empty);

    if (empty) {
        indexLabel->setText("No tutorials available.");
        titleVal->clear(); presenterVal->clear();
        durationVal->clear(); likesVal->clear(); linkVal->clear();
        return;
    }

    const Tutorial& t = all[currentIndex];
    indexLabel->setText(QString("Tutorial %1 of %2")
        .arg(currentIndex + 1).arg(static_cast<int>(all.size())));
    titleVal->setText(QString::fromStdString(t.getTitle()));
    presenterVal->setText(QString::fromStdString(t.getPresenter()));
    durationVal->setText(QString("%1m %2s")
        .arg(t.getDuration().getMinutes())
        .arg(t.getDuration().getSeconds()));
    likesVal->setText(QString::number(t.getLikes()));
    QString link = QString::fromStdString(t.getLink());
    linkVal->setText("<a href=\"" + link + "\">" + link + "</a>");
    linkVal->setOpenExternalLinks(true);
    linkVal->setTextFormat(Qt::RichText);
    linkVal->setTextInteractionFlags(Qt::TextBrowserInteraction);
}

void UserWindow::onPrev() {
    int n = service.length();
    if (n == 0) return;
    currentIndex = (currentIndex - 1 + n) % n;
    showCurrentTutorial();
    statusLabel->clear();
}

void UserWindow::onNext() {
    int n = service.length();
    if (n == 0) return;
    currentIndex = (currentIndex + 1) % n;
    showCurrentTutorial();
    statusLabel->clear();
}

void UserWindow::onAddToWatchlist() {
    auto all = service.get_all();
    if (all.empty()) return;
    const Tutorial& t = all[currentIndex];

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Add to Watchlist",
        QString("Do you want to add \"%1\" to your watchlist?")
        .arg(QString::fromStdString(t.getTitle())),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (watchListService.add(t))
            statusLabel->setText(QString("Added \"%1\" to watchlist.")
                .arg(QString::fromStdString(t.getTitle())));
        else
            statusLabel->setText("Already in watchlist.");
    }
}
void UserWindow::onOpenFile() {
    try {
        watchListService.displayFile();
        statusLabel->setText("Watchlist file opened.");
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}
#include "moc_user_window.cpp"
