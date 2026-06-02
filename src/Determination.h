#pragma once

struct Determination {
    double sampleMass = 0.0;  // Mass of initial sample (g)
    double clayMass = 0.0;    // Mass of clay clumps (g)
    double clayContent = 0.0; // Calculated clay content (%)

    Determination() = default;
    Determination(double sample, double clay)
            : sampleMass(sample), clayMass(clay), clayContent(0) {
    }
};