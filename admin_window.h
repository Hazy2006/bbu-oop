#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "service.h"

class AdminWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit AdminWindow(Service& service, QWidget* parent = nullptr);

private:
    Service& service;

    QTableWidget* table;
    QLineEdit*    titleEdit;
    QLineEdit*    presenterEdit;
    QLineEdit*    minutesEdit;
    QLineEdit*    secondsEdit;
    QLineEdit*    likesEdit;
    QLineEdit*    linkEdit;
    QLineEdit*    filterEdit;
    QPushButton*  addBtn;
    QPushButton*  updateBtn;
    QPushButton*  deleteBtn;
    QPushButton*  filterBtn;
    QPushButton*  clearFilterBtn;
    QLabel*       statusLabel;

    void setupUI();
    void populateTable(const std::vector<Tutorial>& tutorials);
    void refreshTable();
    void clearForm();

private slots:
    void onAdd();
    void onUpdate();
    void onDelete();
    void onFilter();
    void onClearFilter();
    void onRowSelected(int row, int col);
};
