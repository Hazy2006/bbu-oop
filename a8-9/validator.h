#pragma once
#include "domain.h"
#include "exceptions.h"

class TutorialValidator {
public:
    // Validates a Tutorial's fields.
    static void validate(const Tutorial& t);
};