#include "ReportGenerator.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

ReportGenerator::ReportGenerator() : operatorName("Operator") { updateTimestamp(); }

void ReportGenerator::updateTimestamp() {
    time_t now = time(0);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);

    std::stringstream ss;
    ss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S");
    timestamp = ss.str();
}

std::string ReportGenerator::generateReport(const ClayCalculator& calculator, const std::string& complianceStatus) {
    std::stringstream ss;

    ss << "================================================================================\n";
    ss << "                    CLAY CONTENT TEST PROTOCOL\n";
    ss << "                   GOST 8269.0-97, Paragraph 4.6\n";
    ss << "================================================================================\n\n";

    ss << "Date/Time: " << timestamp << "\n";
    ss << "Operator: " << operatorName << "\n\n";

    ss << "MEASUREMENTS:\n";
    ss << "--------------------------------------------------------------------------------\n";
    ss << "No. | Sample Mass (g) | Clay Mass (g) | Clay Content (%) \n";
    ss << "--------------------------------------------------------------------------------\n";

    const auto& determinations = calculator.getDeterminations();
    for (size_t i = 0; i < determinations.size(); ++i) {
        ss << std::setw(3) << (i + 1) << " | ";
        ss << std::setw(15) << std::fixed << std::setprecision(2)
           << determinations[i].sampleMass << " | ";
        ss << std::setw(13) << std::fixed << std::setprecision(2)
           << determinations[i].clayMass << " | ";
        ss << std::setw(17) << std::fixed << std::setprecision(2)
           << determinations[i].clayContent << "\n";
    }

    ss << "--------------------------------------------------------------------------------\n\n";

    ss << "STATISTICS:\n";
    ss << "Average clay content: " << std::fixed << std::setprecision(2)
       << calculator.getAverageContent() << "%\n";
    ss << "Minimum value: " << std::fixed << std::setprecision(2)
       << calculator.getMinContent() << "%\n";
    ss << "Maximum value: " << std::fixed << std::setprecision(2)
       << calculator.getMaxContent() << "%\n";
    ss << "Number of determinations: " << calculator.getCount() << "\n\n";

    ss << "COMPLIANCE CHECK:\n";
    ss << "Standard limits: 0 - 5%\n";
    ss << "Result: " << complianceStatus << "\n\n";

    ss << "================================================================================\n";
    ss << "End of Report\n";
    ss << "================================================================================\n";

    return ss.str();
}

bool ReportGenerator::saveToFile(const std::string& filename, const std::string& reportContent) {
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    file << reportContent;
    file.close();
    return true;
}