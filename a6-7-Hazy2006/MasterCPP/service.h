#pragma once
#include <vector>
#include <string>
#include "repository.h"
#include "exceptions.h"

class Service {
private:
    Repo& r;
public:
    Service(Repo& r);
    void add(const std::string& title, const std::string& presenter, int minutes, int seconds, int likes, const std::string& link);
    void remove(const std::string& title, const std::string& presenter);
    void update(const std::string& title, const std::string& presenter, const std::string& newTitle, const std::string& newPresenter, int newMinutes, int newSeconds, const std::string& newLink);
    void find(const std::string& title, const std::string& presenter);
    int length() const;
    std::vector<Tutorial> get_all() const;
    std::vector<Tutorial> getByPresenter(const std::string& presenter) const;
    void incrementLikes(const std::string& title, const std::string& presenter);
};