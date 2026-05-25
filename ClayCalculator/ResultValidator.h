#pragma once

#include <string>

class ResultValidator {
private:
    static constexpr double MIN_LIMIT = 0.0;
    static constexpr double MAX_LIMIT = 5.0;

public:
    ResultValidator() = default;
    ~ResultValidator() = default;

    // Validate input data
    bool validateSampleMass(double mass);
    bool validateClayMass(double clayMass, double sampleMass);

    // Check compliance with standard
    bool isCompliant(double clayContent);

    // Get limits
    static double getMinLimit() { return MIN_LIMIT; }
    static double getMaxLimit() { return MAX_LIMIT; }
};