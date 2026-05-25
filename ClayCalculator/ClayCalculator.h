#pragma once

#include <vector>

namespace ClayCalculatorApp {

    struct Determination
    {
        double sampleMass;  // Mass of initial sample (g)
        double clayMass;    // Mass of clay clumps (g)
        double clayContent; // Calculated clay content (%)

        Determination() : sampleMass(0), clayMass(0), clayContent(0) {}
        Determination(double sample, double clay)
            : sampleMass(sample), clayMass(clay), clayContent(0) {
        }
    };

    class ClayCalculator
    {
    private:
        std::vector<Determination> determinations;
        double averageContent;

    public:
        ClayCalculator();
        ~ClayCalculator();

        void AddDetermination(double sampleMass, double clayMass);
        bool CalculateAll();
        double GetAverageContent() const;
        double GetClayContent(int index) const;
        double GetMinContent() const;
        double GetMaxContent() const;
        int GetCount() const;
        void Clear();
        const std::vector<Determination>& GetDeterminations() const;
    };

} // namespace ClayCalculatorApp