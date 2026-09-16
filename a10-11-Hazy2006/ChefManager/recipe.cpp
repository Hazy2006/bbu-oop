#include "recipe.h"

Recipe::Recipe(const std::string& name, const std::string& cuisine, int prepTime, const std::vector<std::string>& ingredients)
    : name(name), cuisine(cuisine), prepTime(prepTime), ingredients(ingredients) {}

std::string Recipe::getName() const { return name; }
std::string Recipe::getCuisine() const { return cuisine; }
int Recipe::getPrepTime() const { return prepTime; }
std::vector<std::string> Recipe::getIngredients() const { return ingredients; }

std::string Recipe::getIngredientsAsString() const {
    std::string result = "";
    for (size_t i = 0; i < ingredients.size(); ++i) {
        result += ingredients[i];
        if (i < ingredients.size() - 1) {
            result += ",";
        }
    }
    return result;
}
