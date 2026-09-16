#include "chefwindow.h"

ChefWindow::ChefWindow(Service& service, const Chef& chef, QWidget* parent) 
    : QWidget(parent), service(service), chef(chef) {
    setupUI();
    service.getRepo().attach(this);
    populateList();
}

ChefWindow::~ChefWindow() {
    service.getRepo().detach(this);
}

void ChefWindow::setupUI() {
    setWindowTitle(QString::fromStdString(chef.getName()));
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout* topLayout = new QHBoxLayout();
    filterSpecialtyCheck = new QCheckBox(QString::fromStdString("Filter by my specialty (" + chef.getSpecialty() + ")"));
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search ingredients (comma separated)...");
    topLayout->addWidget(filterSpecialtyCheck);
    topLayout->addWidget(searchEdit);
    
    listWidget = new QListWidget();
    
    nameEdit = new QLineEdit();
    prepTimeEdit = new QLineEdit();
    ingredientsEdit = new QLineEdit();
    
    addRecipeButton = new QPushButton("Add Recipe");
    viewGraphicButton = new QPushButton("View Graphical Recipes");
    
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(listWidget);
    mainLayout->addWidget(new QLabel("Name:"));
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(new QLabel("Prep Time:"));
    mainLayout->addWidget(prepTimeEdit);
    mainLayout->addWidget(new QLabel("Ingredients:"));
    mainLayout->addWidget(ingredientsEdit);
    mainLayout->addWidget(addRecipeButton);
    mainLayout->addWidget(viewGraphicButton);
    
    connect(addRecipeButton, &QPushButton::clicked, this, &ChefWindow::onAddRecipeClicked);
    connect(viewGraphicButton, &QPushButton::clicked, this, &ChefWindow::onViewGraphicClicked);
    connect(filterSpecialtyCheck, &QCheckBox::stateChanged, this, &ChefWindow::onFilterChanged);
    connect(searchEdit, &QLineEdit::textChanged, this, &ChefWindow::onSearchChanged);
}

void ChefWindow::update() {
    populateList();
}

bool ChefWindow::matchIngredients(const Recipe& r, const std::string& query) {
    std::vector<std::string> searchTerms;
    std::stringstream ss(query);
    std::string term;

    while (std::getline(ss, term, ',')) {
        term.erase(0, term.find_first_not_of(" \t"));
        term.erase(term.find_last_not_of(" \t") + 1);
        if (!term.empty()) searchTerms.push_back(term);
    }

    auto recipeIngredients = r.getIngredients();
    for (const auto& sTerm : searchTerms) {
        bool found = false;
        for (const auto& rIng : recipeIngredients) {
            if (rIng == sTerm) { 
                found = true; break; 
            }
        }
        if (!found) return false;
    }
    return true;
}

void ChefWindow::populateList() {
    listWidget->clear();
    auto recipes = service.getRepo().getRecipes();
    
    // Sort Alphabetically by Cuisine
    std::sort(recipes.begin(), recipes.end(), [](const Recipe& a, const Recipe& b) {
        return a.getCuisine() < b.getCuisine();
    });

    for (const auto& r : recipes) {
        if (filterSpecialtyCheck->isChecked() && r.getCuisine() != chef.getSpecialty()) {
            continue;
        }
        
        std::string searchText = searchEdit->text().toStdString();
        if (!searchText.empty() && !matchIngredients(r, searchText)) {
            continue;
        }
        
        std::string itemText = r.getName() + " | " + r.getCuisine() + " | " + std::to_string(r.getPrepTime()) + "m | " + r.getIngredientsAsString();
        listWidget->addItem(QString::fromStdString(itemText));
    }
}

void ChefWindow::onAddRecipeClicked() {
    try {
        service.addRecipe(nameEdit->text().toStdString(), chef.getSpecialty(), prepTimeEdit->text().toStdString(), ingredientsEdit->text().toStdString());
    } catch (ValidationException& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void ChefWindow::onViewGraphicClicked() {
    auto* graphicWin = new GraphicWindow(service, chef.getSpecialty());
    graphicWin->show();
}

void ChefWindow::onFilterChanged() { populateList(); }
void ChefWindow::onSearchChanged() { populateList(); }
