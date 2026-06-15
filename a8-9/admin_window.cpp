#include "admin_window.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QShortcut>
#include <QKeySequence>

AdminWindow::AdminWindow(Service& service, QWidget* parent)
    : QMainWindow(parent), service(service)
{
    setupUI();
    refreshTable();
}

void AdminWindow::setupUI() {
    setWindowTitle("MasterCPP \xe2\x80\x93 Admin");
    resize(900, 700);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // --- Filter bar ---
    QHBoxLayout* filterLayout = new QHBoxLayout;
    filterEdit = new QLineEdit;
    filterEdit->setPlaceholderText("Filter by title or presenter...");
    filterBtn  = new QPushButton("Filter");
    showAllBtn = new QPushButton("Show All");
    filterLayout->addWidget(new QLabel("Filter:"));
    filterLayout->addWidget(filterEdit, 1);
    filterLayout->addWidget(filterBtn);
    filterLayout->addWidget(showAllBtn);
    mainLayout->addLayout(filterLayout);

    // --- Table ---
    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Title", "Presenter", "Min", "Sec", "Likes", "Link"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setAlternatingRowColors(true);
    mainLayout->addWidget(table, 1);

    // --- Form ---
    QGroupBox*   formBox = new QGroupBox("Tutorial Details");
    QGridLayout* grid    = new QGridLayout(formBox);
    titleEdit     = new QLineEdit;
    presenterEdit = new QLineEdit;
    minutesEdit   = new QLineEdit;
    secondsEdit   = new QLineEdit;
    likesEdit     = new QLineEdit;
    linkEdit      = new QLineEdit;

    grid->addWidget(new QLabel("Title:"),     0, 0); grid->addWidget(titleEdit,     0, 1);
    grid->addWidget(new QLabel("Presenter:"), 1, 0); grid->addWidget(presenterEdit, 1, 1);
    grid->addWidget(new QLabel("Minutes:"),   2, 0); grid->addWidget(minutesEdit,   2, 1);
    grid->addWidget(new QLabel("Seconds:"),   3, 0); grid->addWidget(secondsEdit,   3, 1);
    grid->addWidget(new QLabel("Likes:"),     4, 0); grid->addWidget(likesEdit,     4, 1);
    grid->addWidget(new QLabel("Link:"),      5, 0); grid->addWidget(linkEdit,      5, 1);
    grid->setColumnStretch(1, 1);
    mainLayout->addWidget(formBox);

    // --- Action buttons ---
    QHBoxLayout* btnLayout = new QHBoxLayout;
    addBtn    = new QPushButton("Add");
    updateBtn = new QPushButton("Update Selected");
    deleteBtn = new QPushButton("Delete Selected");
    undoBtn   = new QPushButton("Undo (Ctrl+Z)");
    redoBtn   = new QPushButton("Redo (Ctrl+Y)");
    undoBtn->setEnabled(false);
    redoBtn->setEnabled(false);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(undoBtn);
    btnLayout->addWidget(redoBtn);
    mainLayout->addLayout(btnLayout);

    // --- Status ---
    statusLabel = new QLabel;
    mainLayout->addWidget(statusLabel);

    // Connections — buttons
    connect(addBtn,    &QPushButton::clicked, this, &AdminWindow::onAdd);
    connect(updateBtn, &QPushButton::clicked, this, &AdminWindow::onUpdate);
    connect(deleteBtn, &QPushButton::clicked, this, &AdminWindow::onDelete);
    connect(undoBtn,   &QPushButton::clicked, this, &AdminWindow::onUndo);
    connect(redoBtn,   &QPushButton::clicked, this, &AdminWindow::onRedo);
    connect(filterBtn, &QPushButton::clicked, this, &AdminWindow::onFilter);
    connect(showAllBtn,&QPushButton::clicked, this, &AdminWindow::onShowAll);
    connect(table, &QTableWidget::cellClicked, this, &AdminWindow::onRowSelected);

    // Keyboard shortcuts
    auto* undoSc = new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_Z), this);
    connect(undoSc, &QShortcut::activated, this, &AdminWindow::onUndo);

    auto* redoSc = new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_Y), this);
    connect(redoSc, &QShortcut::activated, this, &AdminWindow::onRedo);

    // Delete key only fires when the table widget has focus
    auto* delSc = new QShortcut(QKeySequence(Qt::Key_Delete), table);
    delSc->setContext(Qt::WidgetShortcut);
    connect(delSc, &QShortcut::activated, this, &AdminWindow::onDelete);

    auto* focusTitleSc = new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_N), this);
    connect(focusTitleSc, &QShortcut::activated, this, [this]{ titleEdit->setFocus(); });

    auto* focusFilterSc = new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_F), this);
    connect(focusFilterSc, &QShortcut::activated, this, [this]{ filterEdit->setFocus(); });

    auto* escapeSc = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(escapeSc, &QShortcut::activated, this, [this]{ table->clearSelection(); });
}

void AdminWindow::populateTable(const std::vector<Tutorial>& tutorials) {
    table->setRowCount(0);
    for (const auto& t : tutorials) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(t.getTitle())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(t.getPresenter())));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(t.getDuration().getMinutes())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(t.getDuration().getSeconds())));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(t.getLikes())));
        table->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(t.getLink())));
    }
    statusLabel->setText(QString("Showing %1 tutorial(s).").arg(tutorials.size()));
}

void AdminWindow::refreshTable() {
    populateTable(service.get_all());
    clearForm();
}

void AdminWindow::clearForm() {
    titleEdit->clear();
    presenterEdit->clear();
    minutesEdit->clear();
    secondsEdit->clear();
    likesEdit->clear();
    linkEdit->clear();
}

void AdminWindow::updateUndoRedoButtons() {
    undoBtn->setEnabled(!undoStack.empty());
    redoBtn->setEnabled(!redoStack.empty());
}

// --- Slots ---

void AdminWindow::onRowSelected(int row, int /*col*/) {
    if (row < 0 || row >= table->rowCount()) return;
    titleEdit->setText(table->item(row, 0)->text());
    presenterEdit->setText(table->item(row, 1)->text());
    minutesEdit->setText(table->item(row, 2)->text());
    secondsEdit->setText(table->item(row, 3)->text());
    likesEdit->setText(table->item(row, 4)->text());
    linkEdit->setText(table->item(row, 5)->text());
}

void AdminWindow::onAdd() {
    std::string title     = titleEdit->text().toStdString();
    std::string presenter = presenterEdit->text().toStdString();
    int         minutes   = minutesEdit->text().toInt();
    int         seconds   = secondsEdit->text().toInt();
    int         likes     = likesEdit->text().toInt();
    std::string link      = linkEdit->text().toStdString();
    try {
        service.add(title, presenter, minutes, seconds, likes, link);
        while (!redoStack.empty()) redoStack.pop();
        undoStack.push(std::make_unique<AddAction>(service,
            Tutorial{title, presenter, minutes, seconds, likes, link}));
        updateUndoRedoButtons();
        refreshTable();
        statusLabel->setText("Tutorial added.");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void AdminWindow::onUpdate() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Info", "Select a tutorial to update.");
        return;
    }
    std::string origTitle     = table->item(row, 0)->text().toStdString();
    std::string origPresenter = table->item(row, 1)->text().toStdString();
    int         origMinutes   = table->item(row, 2)->text().toInt();
    int         origSeconds   = table->item(row, 3)->text().toInt();
    int         origLikes     = table->item(row, 4)->text().toInt();
    std::string origLink      = table->item(row, 5)->text().toStdString();

    std::string newTitle     = titleEdit->text().toStdString();
    std::string newPresenter = presenterEdit->text().toStdString();
    int         newMinutes   = minutesEdit->text().toInt();
    int         newSeconds   = secondsEdit->text().toInt();
    std::string newLink      = linkEdit->text().toStdString();

    try {
        service.update(origTitle, origPresenter, newTitle, newPresenter, newMinutes, newSeconds, newLink);
        Tutorial before{origTitle, origPresenter, origMinutes, origSeconds, origLikes, origLink};
        Tutorial after {newTitle,  newPresenter,  newMinutes,  newSeconds,  origLikes, newLink};
        while (!redoStack.empty()) redoStack.pop();
        undoStack.push(std::make_unique<UpdateAction>(service, before, after));
        updateUndoRedoButtons();
        refreshTable();
        statusLabel->setText("Tutorial updated.");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void AdminWindow::onDelete() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Info", "Select a tutorial to delete.");
        return;
    }
    std::string title     = table->item(row, 0)->text().toStdString();
    std::string presenter = table->item(row, 1)->text().toStdString();
    int         minutes   = table->item(row, 2)->text().toInt();
    int         seconds   = table->item(row, 3)->text().toInt();
    int         likes     = table->item(row, 4)->text().toInt();
    std::string link      = table->item(row, 5)->text().toStdString();
    try {
        int originalIndex = -1;
        const auto& all = service.get_all();
        for (int i = 0; i < static_cast<int>(all.size()); ++i) {
            if (all[i].getTitle() == title && all[i].getPresenter() == presenter) {
                originalIndex = i;
                break;
            }
        }
        service.remove(title, presenter);
        while (!redoStack.empty()) redoStack.pop();
        undoStack.push(std::make_unique<RemoveAction>(service,
            Tutorial{title, presenter, minutes, seconds, likes, link}, originalIndex));
        updateUndoRedoButtons();
        refreshTable();
        statusLabel->setText("Tutorial deleted.");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void AdminWindow::onUndo() {
    if (undoStack.empty()) return;
    auto action = std::move(undoStack.top());
    undoStack.pop();
    try {
        action->executeUndo();
        redoStack.push(std::move(action));
        statusLabel->setText("Undo performed.");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Undo Error", e.what());
    }
    updateUndoRedoButtons();
    refreshTable();
}

void AdminWindow::onRedo() {
    if (redoStack.empty()) return;
    auto action = std::move(redoStack.top());
    redoStack.pop();
    try {
        action->executeRedo();
        undoStack.push(std::move(action));
        statusLabel->setText("Redo performed.");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Redo Error", e.what());
    }
    updateUndoRedoButtons();
    refreshTable();
}

void AdminWindow::onFilter() {
    std::string f = filterEdit->text().toStdString();
    if (f.empty()) { populateTable(service.get_all()); return; }
    std::vector<Tutorial> result;
    for (const auto& t : service.get_all())
        if (t.getTitle().find(f) != std::string::npos ||
            t.getPresenter().find(f) != std::string::npos)
            result.push_back(t);
    populateTable(result);
}

void AdminWindow::onShowAll() {
    filterEdit->clear();
    refreshTable();
}

#include "moc_admin_window.cpp"
