#pragma once

#include <vector>
#include <string>
#include "Determination.h"



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