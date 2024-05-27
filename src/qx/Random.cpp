#include "qx/Random.hpp"

#include <random>


namespace qx::random {

namespace {
class RandomNumberGenerator {
public:
    using RandomNumberGeneratorType = std::mt19937_64;

    static RandomNumberGeneratorType &getInstance() {
        static RandomNumberGenerator instance;
        return instance.randomNumberGenerator;
    }

    RandomNumberGenerator(RandomNumberGenerator const &) = delete;

    void operator=(RandomNumberGenerator const &) = delete;

private:
    RandomNumberGenerator() {
        std::random_device rd;
        randomNumberGenerator.seed(rd());
    }

    RandomNumberGeneratorType randomNumberGenerator;
};
} // namespace

void seed(std::uint_fast64_t seedValue) {
    RandomNumberGenerator::getInstance().seed(seedValue);
}

double randomZeroOneDouble() {
    // std::uniform_real_distribution<double> does not give the same result
    // across platforms, so use this instead.

    double result = uniformMinMaxIntegerDistribution(
        0, UINT_FAST64_MAX, static_cast<double>(RandomNumberGenerator::getInstance()()));
    assert(0. <= result && result <= 1.);
    return result;
}

std::uint_fast64_t randomInteger(std::uint_fast64_t min,
                                 std::uint_fast64_t max) {
    // std::uniform_int_distribution<std::uint_fast64_t> is not consistent
    // across platforms.
    assert(min <= max);
    assert(max - min < UINT_FAST64_MAX);

    std::uint_fast64_t numberOfBuckets = max - min + 1;
    std::uint_fast64_t bucketSize = UINT_FAST64_MAX / numberOfBuckets;
    std::uint_fast64_t limit = numberOfBuckets * bucketSize;

    assert(limit == numberOfBuckets * bucketSize);
    assert(limit <= UINT_FAST64_MAX);

    std::uint_fast64_t r;
    do {
        r = RandomNumberGenerator::getInstance()();
    } while (r >= limit);

    std::uint_fast64_t bucketIndex = r / bucketSize;

    assert(bucketIndex < numberOfBuckets);

    std::size_t result = min + bucketIndex;

    assert(min <= result && result <= max);

    return result;
}

double uniformMinMaxIntegerDistribution(std::uint_fast64_t min,
                                        std::uint_fast64_t max, double x) {
    assert(min <= max);

    if (x < static_cast<double>(min)) {
        return 0.;
    }

    if (x >= static_cast<double>(max)) {
        return 1.;
    }

    return (std::floor(x - static_cast<double>(min)) + 1) / (static_cast<double>(max - min) + 1);
}

double uniformZeroOneContinuousDistribution(double x) {
    if (x < 0) {
        return 0;
    }

    if (x > 1) {
        return 1;
    }

    return x;
}

} // namespace qx::random
