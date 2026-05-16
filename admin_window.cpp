#include "admin_window.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>

AdminWindow::AdminWindow(Service& service, QWidget* parent)
    : QMainWindow(parent), service(service)
{
    setupUI();
    refreshTable();
}

void AdminWindow::setupUI() {
    setWindowTitle("MasterCPP \xe2\x80\x93 Admin");
    resize(900, 650);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

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

    // --- Filter row ---
    QHBoxLayout* filterLayout = new QHBoxLayout;
    filterEdit     = new QLineEdit;
    filterEdit->setPlaceholderText("Presenter name...");
    filterBtn      = new QPushButton("Filter");
    clearFilterBtn = new QPushButton("Show All");
    filterLayout->addWidget(new QLabel("Filter by Presenter:"));
    filterLayout->addWidget(filterEdit, 1);
    filterLayout->addWidget(filterBtn);
    filterLayout->addWidget(clearFilterBtn);
    mainLayout->addLayout(filterLayout);

    // --- Form ---
    QGroupBox* formBox    = new QGroupBox("Tutorial Details");
    QGridLayout* grid     = new QGridLayout(formBox);
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
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    mainLayout->addLayout(btnLayout);

    // --- Status bar ---
    statusLabel = new QLabel;
    mainLayout->addWidget(statusLabel);

    connect(addBtn,         &QPushButton::clicked, this, &AdminWindow::onAdd);
    connect(updateBtn,      &QPushButton::clicked, this, &AdminWindow::onUpdate);
    connect(deleteBtn,      &QPushButton::clicked, this, &AdminWindow::onDelete);
    connect(filterBtn,      &QPushButton::clicked, this, &AdminWindow::onFilter);
    connect(clearFilterBtn, &QPushButton::clicked, this, &AdminWindow::onClearFilter);
    connect(table, &QTableWidget::cellClicked, this, &AdminWindow::onRowSelected);
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
    try {
        service.add(
            titleEdit->text().toStdString(),
            presenterEdit->text().toStdString(),
            minutesEdit->text().toInt(),
            secondsEdit->text().toInt(),
            likesEdit->text().toInt(),
            linkEdit->text().toStdString()
        );
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
    // Identity of the tutorial to update comes from the table (works even with filter active)
    std::string origTitle     = table->item(row, 0)->text().toStdString();
    std::string origPresenter = table->item(row, 1)->text().toStdString();
    try {
        service.update(
            origTitle, origPresenter,
            titleEdit->text().toStdString(),
            presenterEdit->text().toStdString(),
            minutesEdit->text().toInt(),
            secondsEdit->text().toInt(),
            linkEdit->text().toStdString()
        );
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
    try {
        service.remove(title, presenter);
        refreshTable();
        statusLabel->setText("Tutorial deleted.");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void AdminWindow::onFilter() {
    populateTable(service.getByPresenter(filterEdit->text().toStdString()));
}

void AdminWindow::onClearFilter() {
    filterEdit->clear();
    refreshTable();
}

#include "moc_admin_window.cpp"
