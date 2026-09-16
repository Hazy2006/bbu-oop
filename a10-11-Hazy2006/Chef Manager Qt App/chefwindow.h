#pragma once
#include <QWidget>
#include <QListWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "observer.h"
#include "service.h"
#include "chef.h"
#include "graphicwindow.h"

class ChefWindow : public QWidget, public Observer {
    Q_OBJECT
private:
    Service& service;
    Chef chef;
    
    QListWidget* listWidget;
    QCheckBox* filterSpecialtyCheck;
    QLineEdit* searchEdit;
    
    QLineEdit* nameEdit;
    QLineEdit* prepTimeEdit;
    QLineEdit* ingredientsEdit;
    
    QPushButton* addRecipeButton;
    QPushButton* viewGraphicButton;
    
    void setupUI();
    void populateList();
    bool matchIngredients(const Recipe& r, const std::string& query);

private slots:
    void onAddRecipeClicked();
    void onViewGraphicClicked();
    void onFilterChanged();
    void onSearchChanged();

public:
    ChefWindow(Service& service, const Chef& chef, QWidget* parent = nullptr);
    ~ChefWindow();
    void update() override;
};
