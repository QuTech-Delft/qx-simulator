#pragma once

#include <random>

namespace qx {
namespace random {

class UniformRandomNumberGenerator {
public:
    UniformRandomNumberGenerator(double min = 0.0f, double max = 1.0f)
        : generator(rd()), distribution(min, max) {}

    double next() { return distribution(generator); }

private:
    std::random_device rd;
    std::mt19937 generator;
    std::uniform_real_distribution<double> distribution;
};

} // namespace random
} // namespace qx