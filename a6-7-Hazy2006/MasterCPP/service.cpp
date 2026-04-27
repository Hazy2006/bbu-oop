#include "service.h"
#include "validator.h"
#include <algorithm>

Service::Service(Repo& r) : r(r) {}

void Service::add(const std::string& title, const std::string& presenter, int minutes, int seconds, int likes, const std::string& link) {
    Tutorial t{ title, presenter, minutes, seconds, likes, link };
    TutorialValidator::validate(t);  // throws ValidationException if invalid
    r.add(t);                        // throws RepositoryException if duplicate
}

void Service::remove(const std::string& title, const std::string& presenter) {
    if (title.empty())     throw ValidationException("Title cannot be empty!");
    if (presenter.empty()) throw ValidationException("Presenter cannot be empty!");
    int index = r.search(Tutorial{ title, presenter });
    if (index == -1)       throw RepositoryException("Tutorial not found!");
    r.remove(index);
}

void Service::update(const std::string& title, const std::string& presenter,
    const std::string& newTitle, const std::string& newPresenter,
    int newMinutes, int newSeconds, const std::string& newLink) {
    if (title.empty())     throw ValidationException("Title to find cannot be empty!");
    if (presenter.empty()) throw ValidationException("Presenter to find cannot be empty!");
    int index = r.search(Tutorial{ title, presenter });
    if (index == -1)       throw RepositoryException("Tutorial not found!");
    // Validate new data before applying
    Tutorial updated{ newTitle, newPresenter, newMinutes, newSeconds, 0, newLink };
    TutorialValidator::validate(updated);
    int preservedLikes = r.get_all()[index].getLikes();
    r.update(Tutorial{ newTitle, newPresenter, newMinutes, newSeconds, preservedLikes, newLink }, index);
}

void Service::find(const std::string& title, const std::string& presenter) {
    if (r.search(Tutorial{ title, presenter }) == -1)
        throw RepositoryException("Tutorial not found!");
}

int Service::length() const {
    return r.length();
}

std::vector<Tutorial> Service::get_all() const {
    return r.get_all();
}

std::vector<Tutorial> Service::getByPresenter(const std::string& presenter) const {
    const auto& all = r.get_all();
    if (presenter.empty()) return all;
    std::vector<Tutorial> result;
    std::copy_if(all.begin(), all.end(), std::back_inserter(result),
        [&presenter](const Tutorial& t) { return t.getPresenter() == presenter; });
    return result;
}

void Service::incrementLikes(const std::string& title, const std::string& presenter) {
    int index = r.search(Tutorial{ title, presenter });
    if (index == -1) throw RepositoryException("Tutorial not found!");
    Tutorial t = r.get_all()[index];
    t.incrementLikes();
    r.update(t, index);
}