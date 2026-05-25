#include "ResultValidator.h"

namespace ClayCalculatorApp {

    ResultValidator::ResultValidator()
    {
    }

    ResultValidator::~ResultValidator()
    {
    }

    bool ResultValidator::ValidateSampleMass(double mass, std::string& errorMsg)
    {
        if (mass <= 0)
        {
            errorMsg = "Sample mass must be positive!";
            return false;
        }
        return true;
    }

    bool ResultValidator::ValidateClayMass(double clayMass, double sampleMass, std::string& errorMsg)
    {
        if (clayMass < 0)
        {
            errorMsg = "Clay mass cannot be negative!";
            return false;
        }
        if (clayMass > sampleMass)
        {
            errorMsg = "Clay mass cannot exceed sample mass!";
            return false;
        }
        return true;
    }

    bool ResultValidator::IsCompliant(double clayContent)
    {
        return clayContent >= MIN_LIMIT && clayContent <= MAX_LIMIT;
    }

    std::string ResultValidator::GetComplianceStatus(double clayContent)
    {
        if (IsCompliant(clayContent))
            return "COMPLIES with standard (0-5%)";
        else
            return "DOES NOT COMPLY with standard (0-5%)";
    }

} // namespace ClayCalculatorApp