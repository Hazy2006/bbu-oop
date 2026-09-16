#pragma once
#include "repository.h"
#include <stdexcept>

class ValidationException : public std::runtime_error {
public:
    ValidationException(const std::string& msg) : std::runtime_error(msg) {}
};

class Service {
private:
    Repository& repo;
public:
    Service(Repository& repo) : repo(repo) {}
    Repository& getRepo() { return repo; }
    void addRecipe(const std::string& name, const std::string& cuisine, const std::string& prepTimeStr, const std::string& ingredientsStr);
};
