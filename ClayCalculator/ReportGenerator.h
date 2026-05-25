#pragma once

#include <string>
#include <vector>
#include "ClayCalculator.h"

namespace ClayCalculatorApp {

    class ReportGenerator
    {
    private:
        std::string operatorName;
        std::string timestamp;

    public:
        ReportGenerator();
        ~ReportGenerator();

        // Set operator info
        void SetOperatorName(const std::string& name) { operatorName = name; }
        void UpdateTimestamp();

        // Generate report
        std::string GenerateReport(const ClayCalculator& calculator,
            const std::string& complianceStatus);

        // Save to file
        bool SaveToFile(const std::string& filename,
            const std::string& reportContent);

        // Get timestamp
        std::string GetTimestamp() const { return timestamp; }
    };

} // namespace ClayCalculatorApp