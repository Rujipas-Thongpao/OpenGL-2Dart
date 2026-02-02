#pragma once
#include <cmath>

namespace NoiseUtils {
    const int MAX_PRIME_INDEX = 10;

    struct NoiseConfig {
        int numOctaves = 7;
        double persistence = 0.5;
        int primeIndex = 0;
    };

    // Declare external primes array
    extern const int primes[MAX_PRIME_INDEX][3];

    // Declare functions (no inline)
    double Noise(int i, int x, int y);
    double SmoothedNoise(int i, int x, int y);
    double Interpolate(double a, double b, double x);
    double InterpolatedNoise(int i, double x, double y);
    double ValueNoise2D(double x, double y, const NoiseConfig& config = NoiseConfig());
}
