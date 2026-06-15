#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <stack>
#include <memory>
#include "service.h"
#include "action.h"

class AdminWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit AdminWindow(Service& service, QWidget* parent = nullptr);

private:
    Service& service;
    std::stack<std::unique_ptr<Action>> undoStack;
    std::stack<std::unique_ptr<Action>> redoStack;

    // Filter bar
    QLineEdit*   filterEdit;
    QPushButton* filterBtn;
    QPushButton* showAllBtn;

    QTableWidget* table;
    QLineEdit*    titleEdit;
    QLineEdit*    presenterEdit;
    QLineEdit*    minutesEdit;
    QLineEdit*    secondsEdit;
    QLineEdit*    likesEdit;
    QLineEdit*    linkEdit;
    QPushButton*  addBtn;
    QPushButton*  updateBtn;
    QPushButton*  deleteBtn;
    QPushButton*  undoBtn;
    QPushButton*  redoBtn;
    QLabel*       statusLabel;

    void setupUI();
    void populateTable(const std::vector<Tutorial>& tutorials);
    void refreshTable();
    void clearForm();
    void updateUndoRedoButtons();

private slots:
    void onAdd();
    void onUpdate();
    void onDelete();
    void onRowSelected(int row, int col);
    void onUndo();
    void onRedo();
    void onFilter();
    void onShowAll();
};
