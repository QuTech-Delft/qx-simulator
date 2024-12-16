#include "qx/random.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <map>

namespace qx::random {

class RandomTest : public ::testing::Test {
protected:
    template <typename F>
    double test_statistic(std::vector<double> const& samples, F&& perfect_distribution) {
        // Given X random variable, perfect_distribution is the function x -> P(X <= x).
        // Assumption: perfect_distribution is non-decreasing, with lim -inf = 0 and lim +inf = 1.

        std::map<double, std::uint64_t> multiplicities;
        for (auto s : samples) {
            ++multiplicities[s];
        }

        double test_statistic = 0.;
        std::uint64_t accumulated_multiplicity = 0;
        for (auto sm : multiplicities) {
            auto const& [sample, multiplicity] = sm;

            static constexpr double const EPSILON = 0.000'000'001;

            test_statistic = std::max(test_statistic,
                std::abs(perfect_distribution(sample - EPSILON) -
                    static_cast<double>(accumulated_multiplicity) / samples.size()));

            accumulated_multiplicity += multiplicity;

            test_statistic = std::max(test_statistic,
                std::abs(perfect_distribution(sample + EPSILON) -
                    static_cast<double>(accumulated_multiplicity) / samples.size()));
        }

        return test_statistic;
    };

    template <typename F>
    void check_kolmogorov_smirnov(std::vector<double> const& samples, F&& perfect_distribution) {
        assert(samples.size() > 50 && "Max test statistic is not correctly approximated for sample sets of small size");
        double max_test_statistic = 1.63 / std::sqrt(samples.size());  // alpha = 0.01

        EXPECT_PRED_FORMAT2(::testing::DoubleLE, test_statistic(samples, perfect_distribution), max_test_statistic);
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
    EXPECT_DOUBLE_EQ(uniform_min_max_integer_distribution(0, 3, -1.), 0.);
    EXPECT_DOUBLE_EQ(uniform_min_max_integer_distribution(0, 3, 0.), 1. / 4);
    EXPECT_DOUBLE_EQ(uniform_min_max_integer_distribution(0, 3, 0.5), 1. / 4);
    EXPECT_DOUBLE_EQ(uniform_min_max_integer_distribution(0, 3, 1.), 2. / 4);
    EXPECT_DOUBLE_EQ(uniform_min_max_integer_distribution(0, 3, 1.1), 2. / 4);
    EXPECT_DOUBLE_EQ(uniform_min_max_integer_distribution(0, 3, 3.), 1.);
    EXPECT_DOUBLE_EQ(uniform_min_max_integer_distribution(0, 3, 3.2), 1.);

    EXPECT_DOUBLE_EQ(uniform_zero_one_continuous_distribution(-0.2), 0.);
    EXPECT_DOUBLE_EQ(uniform_zero_one_continuous_distribution(0.2), 0.2);
    EXPECT_DOUBLE_EQ(uniform_zero_one_continuous_distribution(1.2), 1);
}

TEST_F(RandomTest, test_that_the_test_statistic_is_properly_computed) {
    EXPECT_PRED_FORMAT2(::testing::DoubleLE, test_statistic({ 0.5 }, uniform_zero_one_continuous_distribution), 0.5);
    EXPECT_PRED_FORMAT2(
        ::testing::DoubleLE, test_statistic({ 0.5, 0.7 }, uniform_zero_one_continuous_distribution), 0.5);
    EXPECT_PRED_FORMAT2(
        ::testing::DoubleLE, test_statistic({ 0.1, 0.5, 0.7, 0.8 }, uniform_zero_one_continuous_distribution), 0.25);
    EXPECT_PRED_FORMAT2(::testing::DoubleLE,
        test_statistic({ 0, 1 }, [](double x) { return uniform_min_max_integer_distribution(0, 1, x); }),
        0.);
    EXPECT_PRED_FORMAT2(::testing::DoubleLE,
        test_statistic({ 0, 1, 0, 1 }, [](double x) { return uniform_min_max_integer_distribution(0, 1, x); }),
        0.);
    EXPECT_PRED_FORMAT2(::testing::DoubleLE,
        test_statistic({ 0, 0 }, [](double x) { return uniform_min_max_integer_distribution(0, 1, x); }),
        0.5);
    EXPECT_PRED_FORMAT2(::testing::DoubleLE,
        test_statistic({ 1, 1 }, [](double x) { return uniform_min_max_integer_distribution(0, 1, x); }),
        0.5);
    EXPECT_PRED_FORMAT2(::testing::DoubleLE,
        test_statistic({ 1, 1 }, [](double x) { return uniform_min_max_integer_distribution(1, 5, x); }),
        4. / 5.);
}

TEST_F(RandomTestFirstSeedTest, kolmogorov_smirnov_test_for_uniform_random_double_between_0_and_1) {
    std::size_t sample_size = 100000;
    std::vector<double> samples(sample_size);
    std::generate(samples.begin(), samples.end(), random_zero_one_double);

    check_kolmogorov_smirnov(samples, uniform_zero_one_continuous_distribution);
}

TEST_F(RandomTestSecondSeedTest, kolmogorov_smirnov_test_for_uniform_random_double_between_0_and_1) {
    std::size_t sample_size = 100000;
    std::vector<double> samples(sample_size);
    std::generate(samples.begin(), samples.end(), random_zero_one_double);

    check_kolmogorov_smirnov(samples, uniform_zero_one_continuous_distribution);
}

TEST_F(RandomTestThirdSeedTest, kolmogorov_smirnov_test_for_uniform_random_double_between_0_and_1) {
    std::size_t sample_size = 100000;
    std::vector<double> samples(sample_size);
    std::generate(samples.begin(), samples.end(), random_zero_one_double);

    check_kolmogorov_smirnov(samples, uniform_zero_one_continuous_distribution);
}

TEST_F(RandomTestFirstSeedTest, kolmogorov_smirnov_test_for_random_integer) {
    std::size_t sample_size = 1000000;
    std::size_t min = 63;
    std::size_t max = 125;
    std::vector<double> samples(sample_size);
    std::generate(samples.begin(), samples.end(), [&min, &max]() { return random_integer(min, max); });

    check_kolmogorov_smirnov(
        samples, [&min, &max](double x) { return uniform_min_max_integer_distribution(min, max, x); });
}

TEST_F(RandomTestSecondSeedTest, kolmogorov_smirnov_test_for_random_integer) {
    std::size_t sample_size = 1000000;
    std::size_t min = 63;
    std::size_t max = 125;
    std::vector<double> samples(sample_size);
    std::generate(samples.begin(), samples.end(), [&min, &max]() { return random_integer(min, max); });

    check_kolmogorov_smirnov(
        samples, [&min, &max](double x) { return uniform_min_max_integer_distribution(min, max, x); });
}

TEST_F(RandomTestThirdSeedTest, kolmogorov_smirnov_test_for_random_integer) {
    std::size_t sample_size = 1000000;
    std::size_t min = 63;
    std::size_t max = 125;
    std::vector<double> samples(sample_size);
    std::generate(samples.begin(), samples.end(), [&min, &max]() { return random_integer(min, max); });

    check_kolmogorov_smirnov(
        samples, [&min, &max](double x) { return uniform_min_max_integer_distribution(min, max, x); });
}

}  // namespace qx::random
