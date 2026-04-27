#pragma once
#include "domain.h"
#include "exceptions.h"

class TutorialValidator {
public:
    // Validates a Tutorial's fields.
    // Throws: ValidationException with all accumulated error messages if any field is invalid.
    static void validate(const Tutorial& t);
};