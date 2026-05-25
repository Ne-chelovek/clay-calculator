#pragma once

#include <string>

namespace ClayCalculatorApp {

    class ResultValidator
    {
    private:
        static constexpr double MIN_LIMIT = 0.0;
        static constexpr double MAX_LIMIT = 5.0;

    public:
        ResultValidator();
        ~ResultValidator();

        // Validate input data
        bool ValidateSampleMass(double mass, std::string& errorMsg);
        bool ValidateClayMass(double clayMass, double sampleMass, std::string& errorMsg);

        // Check compliance with standard
        bool IsCompliant(double clayContent);
        std::string GetComplianceStatus(double clayContent);

        // Get limits
        double GetMinLimit() const { return MIN_LIMIT; }
        double GetMaxLimit() const { return MAX_LIMIT; }
    };

} // namespace ClayCalculatorApp