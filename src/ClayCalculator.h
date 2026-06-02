#pragma once

#include <vector>
#include <string>

struct Determination {
    double sampleMass = 0.0;  // Mass of initial sample (g)
    double clayMass = 0.0;    // Mass of clay clumps (g)
    double clayContent = 0.0; // Calculated clay content (%)

    Determination() = default;
    Determination(double sample, double clay)
            : sampleMass(sample), clayMass(clay), clayContent(0) {
    }
};

class ClayCalculator {
private:
    std::vector<Determination> determinations;
    double averageContent;

public:
    ClayCalculator();

    void addDetermination(double sampleMass, double clayMass);
    bool calculateAll();
    double getAverageContent() const;
    double getClayContent(size_t index) const;
    double getMinContent() const;
    double getMaxContent() const;
    std::vector<std::string> getCalculations();
    size_t getCount() const;
    const std::vector<Determination>& getDeterminations() const;
    void clear();
};