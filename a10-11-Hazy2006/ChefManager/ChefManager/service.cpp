#include "service.h"
#include <sstream>

void Service::addRecipe(const std::string& name, const std::string& cuisine, const std::string& prepTimeStr, const std::string& ingredientsStr) {
    if (name.empty()) {
        throw ValidationException("Recipe name cannot be empty.");
    }
    
    auto allRecipes = repo.getRecipes();
    for (const auto& r : allRecipes) {
        if (r.getName() == name) {
            throw ValidationException("A recipe with this name already exists.");
        }
    }
    
    int prepTime;
    try {
        prepTime = std::stoi(prepTimeStr);
    } catch (...) {
        throw ValidationException("Preparation time must be a valid integer.");
    }
    
    std::vector<std::string> ingredients;
    std::stringstream ss(ingredientsStr);
    std::string item;
    while (std::getline(ss, item, ',')) {
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        if (!item.empty()) {
            ingredients.push_back(item);
        }
    }
    
    repo.addRecipe(Recipe(name, cuisine, prepTime, ingredients));
}
