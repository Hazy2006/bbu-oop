#pragma once
#include <string>

class Chef {
private:
    std::string name;
    std::string specialty;
public:
    Chef(const std::string& name, const std::string& specialty) : name(name), specialty(specialty) {}
    std::string getName() const { return name; }
    std::string getSpecialty() const { return specialty; }
};
