#include <QApplication>
#include "repository.h"
#include "service.h"
#include "chefwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    Repository repo;
    repo.loadChefs("chefs.txt");
    repo.loadRecipes("recipes.txt");
    
    Service service(repo);
    
    for (const auto& chef : repo.getChefs()) {
        auto* window = new ChefWindow(service, chef);
        window->show();
    }
    
    int result = a.exec();
    
    // Save on application exit
    repo.saveRecipes("recipes.txt");
    
    return result;
}
