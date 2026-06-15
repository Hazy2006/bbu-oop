#include "user_window.h"
#include "watchlist_model.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QAbstractButton>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDesktopServices>
#include <QFileInfo>
#include <QUrl>
#include <QDir>
#include <QDialog>
#include <QTableView>

UserWindow::UserWindow(Service& service, WatchListService& watchListService, QWidget* parent)
    : QWidget(parent), service(service), watchListService(watchListService), currentIndex(0)
{
    filteredTutorials = service.get_all();
    setupUI();
    showCurrentTutorial();
    if (!filteredTutorials.empty()) {
        const std::string& link = filteredTutorials[currentIndex].getLink();
        if (!link.empty())
            QDesktopServices::openUrl(QUrl(QString::fromStdString(link)));
    }
    refreshWatchlistTable();
}

void UserWindow::setupUI() {
    setWindowTitle("MasterCPP \xe2\x80\x93 User");
    resize(700, 620);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Filter row
    QHBoxLayout* filterLayout = new QHBoxLayout;
    filterEdit = new QLineEdit;
    filterEdit->setPlaceholderText("Presenter name...");
    filterBtn  = new QPushButton("Filter");
    showAllBtn = new QPushButton("Show All");
    filterLayout->addWidget(new QLabel("Filter by Presenter:"));
    filterLayout->addWidget(filterEdit, 1);
    filterLayout->addWidget(filterBtn);
    filterLayout->addWidget(showAllBtn);
    mainLayout->addLayout(filterLayout);

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
    addToWatchlistBtn      = new QPushButton("Add to Watchlist");
    removeFromWatchlistBtn = new QPushButton("Remove from Watchlist");
    openFileBtn            = new QPushButton("Open Watchlist File");
    viewWatchlistBtn       = new QPushButton("View Watchlist (Table)");
    actLayout->addWidget(addToWatchlistBtn);
    actLayout->addWidget(removeFromWatchlistBtn);
    actLayout->addWidget(openFileBtn);
    actLayout->addWidget(viewWatchlistBtn);
    mainLayout->addLayout(actLayout);

    // Status
    statusLabel = new QLabel;
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    // Permanent watchlist table
    QGroupBox* wlBox = new QGroupBox("My Watchlist");
    QVBoxLayout* wlLayout = new QVBoxLayout(wlBox);
    watchlistTable = new QTableWidget(0, 5, wlBox);
    watchlistTable->setHorizontalHeaderLabels({"Title", "Presenter", "Duration", "Likes", "Link"});
    watchlistTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    watchlistTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    watchlistTable->horizontalHeader()->setStretchLastSection(true);
    watchlistTable->setAlternatingRowColors(true);
    watchlistTable->verticalHeader()->setVisible(false);
    wlLayout->addWidget(watchlistTable);
    mainLayout->addWidget(wlBox, 1);

    connect(filterBtn,              &QPushButton::clicked, this, &UserWindow::onFilter);
    connect(showAllBtn,             &QPushButton::clicked, this, &UserWindow::onShowAll);
    connect(prevBtn,                &QPushButton::clicked, this, &UserWindow::onPrev);
    connect(nextBtn,                &QPushButton::clicked, this, &UserWindow::onNext);
    connect(addToWatchlistBtn,      &QPushButton::clicked, this, &UserWindow::onAddToWatchlist);
    connect(removeFromWatchlistBtn, &QPushButton::clicked, this, &UserWindow::onRemoveFromWatchlist);
    connect(openFileBtn,            &QPushButton::clicked, this, &UserWindow::onOpenFile);
    connect(viewWatchlistBtn,       &QPushButton::clicked, this, &UserWindow::onViewWatchlist);
}

void UserWindow::showCurrentTutorial() {
    bool empty = filteredTutorials.empty();

    prevBtn->setEnabled(!empty);
    nextBtn->setEnabled(!empty);
    addToWatchlistBtn->setEnabled(!empty);
    removeFromWatchlistBtn->setEnabled(!empty);

    if (empty) {
        indexLabel->setText("No tutorials available.");
        titleVal->clear(); presenterVal->clear();
        durationVal->clear(); likesVal->clear(); linkVal->clear();
        return;
    }

    if (currentIndex >= static_cast<int>(filteredTutorials.size()))
        currentIndex = static_cast<int>(filteredTutorials.size()) - 1;

    const Tutorial& t = filteredTutorials[currentIndex];
    indexLabel->setText(QString("Tutorial %1 of %2")
        .arg(currentIndex + 1).arg(static_cast<int>(filteredTutorials.size())));
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

void UserWindow::refreshWatchlistTable() {
    const std::vector<Tutorial>& wl = watchListService.getAll();
    watchlistTable->setRowCount(static_cast<int>(wl.size()));
    for (int i = 0; i < static_cast<int>(wl.size()); ++i) {
        const Tutorial& wt = wl[i];
        watchlistTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(wt.getTitle())));
        watchlistTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(wt.getPresenter())));
        watchlistTable->setItem(i, 2, new QTableWidgetItem(
            QString("%1m %2s").arg(wt.getDuration().getMinutes()).arg(wt.getDuration().getSeconds())));
        watchlistTable->setItem(i, 3, new QTableWidgetItem(QString::number(wt.getLikes())));
        watchlistTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(wt.getLink())));
    }
    watchlistTable->resizeColumnsToContents();
}

void UserWindow::onFilter() {
    filteredTutorials = service.getByPresenter(filterEdit->text().toStdString());
    currentIndex = 0;
    showCurrentTutorial();
    statusLabel->clear();
}

void UserWindow::onShowAll() {
    filterEdit->clear();
    filteredTutorials = service.get_all();
    currentIndex = 0;
    showCurrentTutorial();
    statusLabel->clear();
}

void UserWindow::onPrev() {
    int n = static_cast<int>(filteredTutorials.size());
    if (n == 0) return;
    currentIndex = (currentIndex - 1 + n) % n;
    showCurrentTutorial();
    statusLabel->clear();
    const std::string& link = filteredTutorials[currentIndex].getLink();
    if (!link.empty())
        QDesktopServices::openUrl(QUrl(QString::fromStdString(link)));
}

void UserWindow::onNext() {
    int n = static_cast<int>(filteredTutorials.size());
    if (n == 0) return;
    currentIndex = (currentIndex + 1) % n;
    showCurrentTutorial();
    statusLabel->clear();
    const std::string& link = filteredTutorials[currentIndex].getLink();
    if (!link.empty())
        QDesktopServices::openUrl(QUrl(QString::fromStdString(link)));
}

void UserWindow::onAddToWatchlist() {
    if (filteredTutorials.empty()) return;
    const Tutorial& t = filteredTutorials[currentIndex];

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Add to Watchlist",
        QString("Do you want to add \"%1\" to your watchlist?")
        .arg(QString::fromStdString(t.getTitle())),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (watchListService.add(t)) {
            statusLabel->setText(QString("Added \"%1\" to watchlist.")
                .arg(QString::fromStdString(t.getTitle())));
            refreshWatchlistTable();
        } else {
            statusLabel->setText("Already in watchlist.");
        }
    }
}

void UserWindow::onRemoveFromWatchlist() {
    if (filteredTutorials.empty()) return;

    // Copy the data we need before any mutations
    const std::string title     = filteredTutorials[currentIndex].getTitle();
    const std::string presenter = filteredTutorials[currentIndex].getPresenter();
    const Tutorial    tutCopy   = filteredTutorials[currentIndex];

    // Search in watchlist
    const std::vector<Tutorial>& wl = watchListService.getAll();
    int wlIndex = -1;
    for (int i = 0; i < static_cast<int>(wl.size()); ++i) {
        if (wl[i] == tutCopy) { wlIndex = i; break; }
    }

    if (wlIndex == -1) {
        statusLabel->setText("This tutorial is not in your watchlist.");
        return;
    }

    watchListService.remove(wlIndex);
    refreshWatchlistTable();

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Like Tutorial",
        "Do you want to like this tutorial?",
        QMessageBox::Yes | QMessageBox::No);

    // Always refresh the filtered list so the display reflects the latest data.
    auto refreshFiltered = [&]() {
        std::string presFilter = filterEdit->text().toStdString();
        filteredTutorials = presFilter.empty()
            ? service.get_all()
            : service.getByPresenter(presFilter);
        showCurrentTutorial();
    };

    if (reply == QMessageBox::Yes) {
        try {
            service.incrementLikes(title, presenter);
            refreshFiltered();
            statusLabel->setText("Removed from watchlist and liked.");
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    } else {
        refreshFiltered();
        statusLabel->setText("Removed from watchlist.");
    }
}

void UserWindow::onOpenFile() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Open Watchlist File");
    msgBox.setText("Choose which watchlist file to open:");
    QPushButton* csvBtn  = msgBox.addButton("CSV",  QMessageBox::AcceptRole);
    QPushButton* htmlBtn = msgBox.addButton("HTML", QMessageBox::AcceptRole);
    msgBox.addButton(QMessageBox::Cancel);
    msgBox.exec();

    QAbstractButton* clicked = msgBox.clickedButton();
    QString path;
    if (clicked == csvBtn)
        path = QDir::current().absoluteFilePath("watchlist.csv");
    else if (clicked == htmlBtn)
        path = QDir::current().absoluteFilePath("watchlist.html");
    else
        return;

    if (!QFileInfo::exists(path)) {
        QMessageBox::warning(this, "File Not Found",
            "The watchlist file does not exist yet:\n" + path +
            "\n\nAdd some tutorials to the watchlist first.");
        return;
    }
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(path)))
        QMessageBox::warning(this, "Error", "Could not open: " + path);
    else
        statusLabel->setText("Opened: " + path);
}

void UserWindow::onViewWatchlist() {
    QDialog dlg(this);
    dlg.setWindowTitle("Watchlist \xe2\x80\x93 Table View");
    dlg.resize(700, 400);

    auto* model = new WatchListModel(watchListService, &dlg);
    auto* view  = new QTableView(&dlg);
    view->setModel(model);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setAlternatingRowColors(true);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setStretchLastSection(true);
    view->resizeColumnsToContents();

    auto* layout = new QVBoxLayout(&dlg);
    layout->addWidget(view);

    dlg.exec();
}

#include "moc_user_window.cpp"
