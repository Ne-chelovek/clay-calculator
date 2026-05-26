#include "ResultValidator.h"
#include "Exceptions.h"

bool ResultValidator::validateSampleMass(double mass) {
    if (mass <= 0)
        throw NegativeMassException("Sample mass cannot be negative!");
    return true;
}

bool ResultValidator::validateClayMass(double clayMass, double sampleMass) {
    if (clayMass < 0)
        throw NegativeMassException("Clay mass cannot be negative!");
    if (clayMass > sampleMass)
        throw MassExceedException("Clay mass cannot exceed sample mass!");
    return true;
}

bool ResultValidator::isCompliant(double clayContent) {
    return clayContent >= MIN_LIMIT && clayContent <= MAX_LIMIT;
}
