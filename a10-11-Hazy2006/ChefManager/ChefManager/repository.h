#pragma once
#include "observer.h"
#include "recipe.h"
#include "chef.h"
#include <vector>
#include <string>

class Repository : public Subject {
private:
    std::vector<Recipe> recipes;
    std::vector<Chef> chefs;
public:
    void loadRecipes(const std::string& filename);
    void loadChefs(const std::string& filename);
    void saveRecipes(const std::string& filename);
    
    void addRecipe(const Recipe& recipe);
    std::vector<Recipe> getRecipes() const;
    std::vector<Chef> getChefs() const;
};
