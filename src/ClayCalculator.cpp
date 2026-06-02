#include "ClayCalculator.h"
#include "Determination.h"
#include <cmath>
#include <sstream>
#include <iomanip>

using std::vector, std::string, std::to_string, std::ostringstream;


ClayCalculator::ClayCalculator() : averageContent(0) {}

void ClayCalculator::addDetermination(double sampleMass, double clayMass) {
    determinations.push_back(Determination(sampleMass, clayMass));
}

bool ClayCalculator::calculateAll() {
    if (determinations.empty()) return false;

    double sum = 0;
    for (auto& det : determinations) {
        if (det.sampleMass <= 0) return false;
        det.clayContent = (det.clayMass / det.sampleMass) * 100.0;
        sum += det.clayContent;
    }

    averageContent = sum / determinations.size();
    return true;
}

double ClayCalculator::getAverageContent() const {
    return averageContent;
}

double ClayCalculator::getClayContent(size_t index) const {
    if (index < determinations.size())
        return determinations[index].clayContent;
    return 0;
}

double ClayCalculator::getMinContent() const {
    if (determinations.empty()) return 0;
    double minVal = determinations[0].clayContent;
    for (const auto& det : determinations)
        if (det.clayContent < minVal)
            minVal = det.clayContent;
    return minVal;
}

double ClayCalculator::getMaxContent() const {
    if (determinations.empty()) return 0;
    double maxVal = determinations[0].clayContent;
    for (const auto& det : determinations)
        if (det.clayContent > maxVal)
            maxVal = det.clayContent;
    return maxVal;
}

size_t ClayCalculator::getCount() const {
    return determinations.size();
}

const std::vector<Determination>& ClayCalculator::getDeterminations() const {
    return determinations;
}

void ClayCalculator::clear() {
    determinations.clear();
    averageContent = 0;
}

vector<string> ClayCalculator::getCalculations() {
    vector<string> results;
    for (const auto& det : determinations) {
        ostringstream oss;
        oss << std::fixed << std::setprecision(1)
            << det.clayMass << " / "
            << det.sampleMass  <<
            " * 100%"<< " = "
            << det.clayContent << "%";
        results.push_back(oss.str());
    }
    return results;
}
