#include "validator.h"
#include <string>

void TutorialValidator::validate(const Tutorial& t) {
    std::string errors;

    if (t.getTitle().empty())
        errors += "Title cannot be empty!\n";
    if (t.getPresenter().empty())
        errors += "Presenter cannot be empty!\n";
    if (t.getDuration().getMinutes() < 0)
        errors += "Minutes cannot be negative!\n";
    if (t.getDuration().getSeconds() < 0 || t.getDuration().getSeconds() > 59)
        errors += "Seconds must be between 0 and 59!\n";
    if (t.getLikes() < 0)
        errors += "Likes cannot be negative!\n";
    if (t.getLink().empty())
        errors += "Link cannot be empty!\n";

    if (!errors.empty())
        throw ValidationException(errors);
}