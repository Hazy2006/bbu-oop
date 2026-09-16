#include "repository.h"
#include <fstream>
#include <sstream>

void Repository::loadRecipes(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, cuisine, prepTimeStr, ingredientsStr;
        
        std::getline(ss, name, '|');
        std::getline(ss, cuisine, '|');
        std::getline(ss, prepTimeStr, '|');
        std::getline(ss, ingredientsStr, '|');
        
        std::vector<std::string> ingredients;
        std::stringstream ingStream(ingredientsStr);
        std::string item;
        while (std::getline(ingStream, item, ',')) {
            item.erase(0, item.find_first_not_of(" \t"));
            item.erase(item.find_last_not_of(" \t") + 1);
            if (!item.empty()) ingredients.push_back(item);
        }
        recipes.push_back(Recipe(name, cuisine, std::stoi(prepTimeStr), ingredients));
    }
}

void Repository::loadChefs(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, specialty;
        std::getline(ss, name, '|');
        std::getline(ss, specialty, '|');
        chefs.push_back(Chef(name, specialty));
    }
}

void Repository::saveRecipes(const std::string& filename) {
    std::sort(recipes.begin(), recipes.end(), [](const Recipe& a, const Recipe& b) {
        if (a.getCuisine() != b.getCuisine()) return a.getCuisine() < b.getCuisine();
        return a.getPrepTime() < b.getPrepTime();
    });

    std::ofstream file(filename);
    for (const auto& r : recipes) {
        file << r.getName() << "|" << r.getCuisine() << "|" << r.getPrepTime() << "|" << r.getIngredientsAsString() << "\n";
    }
}

void Repository::addRecipe(const Recipe& recipe) {
    recipes.push_back(recipe);
    notify(); // Broadcast to all open windows!
}

std::vector<Recipe> Repository::getRecipes() const { return recipes; }
std::vector<Chef> Repository::getChefs() const { return chefs; }
