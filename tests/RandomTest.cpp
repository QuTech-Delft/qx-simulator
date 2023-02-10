#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/Random.hpp"
#include "doctest/doctest.h"
#include <functional>

#define CREATE_SEED_SUBCASES()                                                 \
    do {                                                                       \
        SUBCASE("First seed") { ::qx::random::seed(123456); }                  \
        SUBCASE("Second seed") { ::qx::random::seed(654321); }                 \
        SUBCASE("Third seed") { ::qx::random::seed(11); }                      \
    } while (false)

namespace qx {
namespace random {

class RandomTest {
protected:
    template <typename F>
    double testStatistic(std::vector<double> const &samples,
                         F &&perfectDistribution) {
        // Given X random variable, perfectDistribution is the function x -> P(X
        // <= x). Assumption: perfectDistribution is non-decreasing, with lim
        // -inf = 0 and lim +inf = 1.

        std::map<double, std::uint64_t> multiplicities;
        for (auto s : samples) {
            ++multiplicities[s];
        }

        double testStatistic = 0.;
        std::uint64_t accumulatedMultiplicity = 0;
        for (auto sm : multiplicities) {
            auto const &[sample, multiplicity] = sm;

            static constexpr double const EPSILON = 0.000000001;

            testStatistic =
                std::max(testStatistic,
                         std::abs(perfectDistribution(sample - EPSILON) -
                                  static_cast<double>(accumulatedMultiplicity) /
                                      samples.size()));

            accumulatedMultiplicity += multiplicity;

            testStatistic =
                std::max(testStatistic,
                         std::abs(perfectDistribution(sample + EPSILON) -
                                  static_cast<double>(accumulatedMultiplicity) /
                                      samples.size()));
        }

        return testStatistic;
    };

    template <typename F>
    void checkKolmogorovSmirnov(std::vector<double> const &samples,
                                F &&perfectDistribution) {
        assert(samples.size() > 50 &&
               "Max test statistic is not correctly approximated for sample "
               "sets of small size");
        double maxTestStatistic =
            1.63 / std::sqrt(samples.size()); // alpha = 0.01

        CHECK(testStatistic(samples, perfectDistribution) <=
              doctest::Approx(maxTestStatistic));
    }
};

TEST_CASE_FIXTURE(RandomTest,
                  "Test that the probability distributions are correct") {
    CHECK(uniformMinMaxIntegerDistribution(0, 3, -1.) == doctest::Approx(0.));
    CHECK(uniformMinMaxIntegerDistribution(0, 3, 0.) ==
          doctest::Approx(1. / 4));
    CHECK(uniformMinMaxIntegerDistribution(0, 3, 0.5) ==
          doctest::Approx(1. / 4));
    CHECK(uniformMinMaxIntegerDistribution(0, 3, 1.) ==
          doctest::Approx(2. / 4));
    CHECK(uniformMinMaxIntegerDistribution(0, 3, 1.1) ==
          doctest::Approx(2. / 4));
    CHECK(uniformMinMaxIntegerDistribution(0, 3, 3.) == doctest::Approx(1.));
    CHECK(uniformMinMaxIntegerDistribution(0, 3, 3.2) == doctest::Approx(1.));

    CHECK(uniformZeroOneContinuousDistribution(-0.2) == doctest::Approx(0.));
    CHECK(uniformZeroOneContinuousDistribution(0.2) == doctest::Approx(0.2));
    CHECK(uniformZeroOneContinuousDistribution(1.2) == doctest::Approx(1));
}

TEST_CASE_FIXTURE(RandomTest,
                  "Test that the test statistic is properly computed") {
    CHECK(testStatistic({0.5}, uniformZeroOneContinuousDistribution) ==
          doctest::Approx(0.5));

    CHECK(testStatistic({0.5, 0.7}, uniformZeroOneContinuousDistribution) ==
          doctest::Approx(0.5));

    CHECK(testStatistic({0.1, 0.5, 0.7, 0.8},
                        uniformZeroOneContinuousDistribution) ==
          doctest::Approx(0.25));

    CHECK(testStatistic({0, 1}, std::bind(uniformMinMaxIntegerDistribution, 0,
                                          1, std::placeholders::_1)) ==
          doctest::Approx(0.));
    CHECK(
        testStatistic({0, 1, 0, 1}, std::bind(uniformMinMaxIntegerDistribution,
                                              0, 1, std::placeholders::_1)) ==
        doctest::Approx(0.));
    CHECK(testStatistic({0, 0}, std::bind(uniformMinMaxIntegerDistribution, 0,
                                          1, std::placeholders::_1)) ==
          doctest::Approx(0.5));
    CHECK(testStatistic({1, 1}, std::bind(uniformMinMaxIntegerDistribution, 0,
                                          1, std::placeholders::_1)) ==
          doctest::Approx(0.5));
    CHECK(testStatistic({1, 1}, std::bind(uniformMinMaxIntegerDistribution, 1,
                                          5, std::placeholders::_1)) ==
          doctest::Approx(4. / 5.));
}

TEST_CASE_FIXTURE(
    RandomTest,
    "Kolmogorov-Smirnov test for uniform random double between 0 and 1") {
    CREATE_SEED_SUBCASES();

    std::size_t sampleSize = 100000;
    std::vector<double> samples;
    samples.reserve(sampleSize);
    for (std::size_t i = 0; i < sampleSize; ++i) {
        samples.push_back(randomZeroOneDouble());
    }

    checkKolmogorovSmirnov(samples, uniformZeroOneContinuousDistribution);
}

TEST_CASE_FIXTURE(RandomTest, "Kolmogorov-Smirnov test for random integer") {
    CREATE_SEED_SUBCASES();

    std::size_t sampleSize = 1000000;
    std::size_t min = 63;
    std::size_t max = 125;
    std::vector<double> samples;
    samples.reserve(sampleSize);
    for (std::size_t i = 0; i < sampleSize; ++i) {
        samples.push_back(randomInteger(min, max));
    }

    checkKolmogorovSmirnov(samples, std::bind(uniformMinMaxIntegerDistribution,
                                              min, max, std::placeholders::_1));
}

} // namespace random
} // namespace qx