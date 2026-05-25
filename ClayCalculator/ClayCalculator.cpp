#include "ClayCalculator.h"
#include <cmath>

namespace ClayCalculatorApp {

    ClayCalculator::ClayCalculator() : averageContent(0)
    {
    }

    ClayCalculator::~ClayCalculator()
    {
        Clear();
    }

    void ClayCalculator::AddDetermination(double sampleMass, double clayMass)
    {
        determinations.push_back(Determination(sampleMass, clayMass));
    }

    bool ClayCalculator::CalculateAll()
    {
        if (determinations.empty())
            return false;

        double sum = 0;
        for (auto& det : determinations)
        {
            if (det.sampleMass > 0)
                det.clayContent = (det.clayMass / det.sampleMass) * 100.0;
            else
                return false;
            sum += det.clayContent;
        }

        averageContent = sum / determinations.size();
        return true;
    }

    double ClayCalculator::GetAverageContent() const
    {
        return averageContent;
    }

    double ClayCalculator::GetClayContent(int index) const
    {
        if (index >= 0 && index < (int)determinations.size())
            return determinations[index].clayContent;
        return 0;
    }

    double ClayCalculator::GetMinContent() const
    {
        if (determinations.empty())
            return 0;
        double minVal = determinations[0].clayContent;
        for (const auto& det : determinations)
            if (det.clayContent < minVal)
                minVal = det.clayContent;
        return minVal;
    }

    double ClayCalculator::GetMaxContent() const
    {
        if (determinations.empty())
            return 0;
        double maxVal = determinations[0].clayContent;
        for (const auto& det : determinations)
            if (det.clayContent > maxVal)
                maxVal = det.clayContent;
        return maxVal;
    }

    int ClayCalculator::GetCount() const
    {
        return (int)determinations.size();
    }

    void ClayCalculator::Clear()
    {
        determinations.clear();
        averageContent = 0;
    }

    const std::vector<Determination>& ClayCalculator::GetDeterminations() const
    {
        return determinations;
    }

} // namespace ClayCalculatorApp