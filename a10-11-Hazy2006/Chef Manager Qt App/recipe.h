#pragma once
#include <string>
#include <vector>

class Recipe {
private:
    std::string name;
    std::string cuisine;
    int prepTime;
    std::vector<std::string> ingredients;

public:
    Recipe(const std::string& name, const std::string& cuisine, int prepTime, const std::vector<std::string>& ingredients);
    
    std::string getName() const;
    std::string getCuisine() const;
    int getPrepTime() const;
    std::vector<std::string> getIngredients() const;
    std::string getIngredientsAsString() const;
};
