#include "qx/Random.hpp"

#include <algorithm>
#include <gtest/gtest.h>


namespace qx::random {

class RandomTest : public ::testing::Test {
protected:
    template <typename F>
    double testStatistic(std::vector<double> const &samples, F &&perfectDistribution) {
        // Given X random variable, perfectDistribution is the function x -> P(X <= x).
        // Assumption: perfectDistribution is non-decreasing, with lim -inf = 0 and lim +inf = 1.

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
    void checkKolmogorovSmirnov(std::vector<double> const &samples, F &&perfectDistribution) {
        assert(samples.size() > 50 &&
               "Max test statistic is not correctly approximated for sample sets of small size");
        double maxTestStatistic = 1.63 / std::sqrt(samples.size()); // alpha = 0.01

        EXPECT_PRED_FORMAT2(::testing::DoubleLE,
            testStatistic(samples, perfectDistribution), maxTestStatistic);
    }
};

class RandomTestFirstSeedTest : public RandomTest {
protected:
    void SetUp() override { ::qx::random::seed(123456); }
};

class RandomTestSecondSeedTest : public RandomTest {
protected:
    void SetUp() override { ::qx::random::seed(654321); }
};

class RandomTestThirdSeedTest : public RandomTest {
protected:
    void SetUp() override { ::qx::random::seed(11); }
};

TEST_F(RandomTest, test_that_the_probability_distributions_are_correct) {
    EXPECT_DOUBLE_EQ(uniformMinMaxIntegerDistribution(0, 3, -1.), 0.);
    EXPECT_DOUBLE_EQ(uniformMinMaxIntegerDistribution(0, 3, 0.), 1. / 4);
    EXPECT_DOUBLE_EQ(uniformMinMaxIntegerDistribution(0, 3, 0.5), 1. / 4);
    EXPECT_DOUBLE_EQ(uniformMinMaxIntegerDistribution(0, 3, 1.), 2. / 4);
    EXPECT_DOUBLE_EQ(uniformMinMaxIntegerDistribution(0, 3, 1.1), 2. / 4);
    EXPECT_DOUBLE_EQ(uniformMinMaxIntegerDistribution(0, 3, 3.), 1.);
    EXPECT_DOUBLE_EQ(uniformMinMaxIntegerDistribution(0, 3, 3.2), 1.);

    EXPECT_DOUBLE_EQ(uniformZeroOneContinuousDistribution(-0.2), 0.);
    EXPECT_DOUBLE_EQ(uniformZeroOneContinuousDistribution(0.2), 0.2);
    EXPECT_DOUBLE_EQ(uniformZeroOneContinuousDistribution(1.2), 1);
}

TEST_F(RandomTest, test_that_the_test_statistic_is_properly_computed) {
    EXPECT_DOUBLE_EQ(
        testStatistic({0.5}, uniformZeroOneContinuousDistribution),
        0.5);
    EXPECT_DOUBLE_EQ(
        testStatistic({0.5, 0.7}, uniformZeroOneContinuousDistribution),
        0.5);
    EXPECT_DOUBLE_EQ(
        testStatistic({0.1, 0.5, 0.7, 0.8}, uniformZeroOneContinuousDistribution),
        0.25);
    EXPECT_DOUBLE_EQ(
        testStatistic({0, 1}, [](double x) { return uniformMinMaxIntegerDistribution(0, 1, x); }),
        0.);
    EXPECT_DOUBLE_EQ(
        testStatistic({0, 1, 0, 1}, [](double x) { return uniformMinMaxIntegerDistribution(0, 1, x); }),
        0.);
    EXPECT_DOUBLE_EQ(
        testStatistic({0, 0}, [](double x) { return uniformMinMaxIntegerDistribution(0, 1, x); }),
        0.5);
    EXPECT_DOUBLE_EQ(
        testStatistic({1, 1}, [](double x) { return uniformMinMaxIntegerDistribution(0, 1, x); }),
        0.5);
    EXPECT_DOUBLE_EQ(
        testStatistic({1, 1}, [](double x) { return uniformMinMaxIntegerDistribution(1, 5, x); }),
        4. / 5.);
}

TEST_F(RandomTestFirstSeedTest, kolmogorov_smirnov_test_for_uniform_random_double_between_0_and_1) {
    std::size_t sampleSize = 100000;
    std::vector<double> samples(sampleSize);
    std::generate(samples.begin(), samples.end(), randomZeroOneDouble);

    checkKolmogorovSmirnov(samples, uniformZeroOneContinuousDistribution);
}

TEST_F(RandomTestSecondSeedTest, kolmogorov_smirnov_test_for_uniform_random_double_between_0_and_1) {
    std::size_t sampleSize = 100000;
    std::vector<double> samples(sampleSize);
    std::generate(samples.begin(), samples.end(), randomZeroOneDouble);

    checkKolmogorovSmirnov(samples, uniformZeroOneContinuousDistribution);
}

TEST_F(RandomTestThirdSeedTest, kolmogorov_smirnov_test_for_uniform_random_double_between_0_and_1) {
    std::size_t sampleSize = 100000;
    std::vector<double> samples(sampleSize);
    std::generate(samples.begin(), samples.end(), randomZeroOneDouble);

    checkKolmogorovSmirnov(samples, uniformZeroOneContinuousDistribution);
}

TEST_F(RandomTestFirstSeedTest, kolmogorov_smirnov_test_for_random_integer) {
    std::size_t sampleSize = 1000000;
    std::size_t min = 63;
    std::size_t max = 125;
    std::vector<double> samples(sampleSize);
    std::generate(samples.begin(), samples.end(), [&min, &max]() { return randomInteger(min, max); });

    checkKolmogorovSmirnov(samples,
        [&min, &max](double x) { return uniformMinMaxIntegerDistribution(min, max, x); });
}

TEST_F(RandomTestSecondSeedTest, kolmogorov_smirnov_test_for_random_integer) {
    std::size_t sampleSize = 1000000;
    std::size_t min = 63;
    std::size_t max = 125;
    std::vector<double> samples(sampleSize);
    std::generate(samples.begin(), samples.end(), [&min, &max]() { return randomInteger(min, max); });

    checkKolmogorovSmirnov(samples,
        [&min, &max](double x) { return uniformMinMaxIntegerDistribution(min, max, x); });
}

TEST_F(RandomTestThirdSeedTest, kolmogorov_smirnov_test_for_random_integer) {
    std::size_t sampleSize = 1000000;
    std::size_t min = 63;
    std::size_t max = 125;
    std::vector<double> samples(sampleSize);
    std::generate(samples.begin(), samples.end(), [&min, &max]() { return randomInteger(min, max); });

    checkKolmogorovSmirnov(samples,
        [&min, &max](double x) { return uniformMinMaxIntegerDistribution(min, max, x); });
}

} // namespace qx::random
