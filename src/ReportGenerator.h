#pragma once

#include <string>
#include <vector>
#include "ClayCalculator.h"

class ReportGenerator {
private:
    std::string operatorName;
    std::string timestamp;

public:
    ReportGenerator();

    void setOperatorName(const std::string& name) { operatorName = name; }

    void updateTimestamp();

    std::string generateReport(const ClayCalculator& calculator,
                               const std::string& complianceStatus);

    bool saveToFile(const std::string& filename,
                    const std::string& reportContent);
};