#include "ResultValidator.h"

#include <iostream>

bool ResultValidator::validateSampleMass(double mass) {
    if (mass <= 0)
        throw "Sample mass must be positive!";
    return true;
}

bool ResultValidator::validateClayMass(double clayMass, double sampleMass) {
    if (clayMass < 0)
        throw "Clay mass cannot be negative!";
    if (clayMass > sampleMass)
        throw "Clay mass cannot exceed sample mass!";
    return true;
}

bool ResultValidator::isCompliant(double clayContent) {
    return clayContent >= MIN_LIMIT && clayContent <= MAX_LIMIT;
}
