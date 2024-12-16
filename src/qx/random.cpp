#include "qx/random.hpp"

#include <random>

namespace qx::random {

namespace {
class RandomNumberGenerator {
public:
    using RandomNumberGeneratorType = std::mt19937_64;

    static RandomNumberGeneratorType& get_instance() {
        static RandomNumberGenerator instance;
        return instance.random_number_generator;
    }

    RandomNumberGenerator(RandomNumberGenerator const&) = delete;

    void operator=(RandomNumberGenerator const&) = delete;

private:
    RandomNumberGenerator() { random_number_generator.seed(std::random_device{}()); }

    RandomNumberGeneratorType random_number_generator;
};
}  // namespace

void seed(std::uint_fast64_t seed_value) {
    RandomNumberGenerator::get_instance().seed(seed_value);
}

double random_zero_one_double() {
    // std::uniform_real_distribution<double> does not give the same result
    // across platforms, so use this instead.

    double result = uniform_min_max_integer_distribution(
        0, UINT_FAST64_MAX, static_cast<double>(RandomNumberGenerator::get_instance()()));

    assert(0. <= result && result <= 1.);

    return result;
}

std::uint_fast64_t random_integer(std::uint_fast64_t min, std::uint_fast64_t max) {
    // std::uniform_int_distribution<std::uint_fast64_t> is not consistent across platforms.
    assert(min <= max);
    assert(max - min < UINT_FAST64_MAX);

    std::uint_fast64_t number_of_buckets = max - min + 1;
    std::uint_fast64_t bucket_size = UINT_FAST64_MAX / number_of_buckets;
    std::uint_fast64_t limit = number_of_buckets * bucket_size;

    assert(limit == number_of_buckets * bucket_size);
    assert(limit <= UINT_FAST64_MAX);

    std::uint_fast64_t r;
    do {
        r = RandomNumberGenerator::get_instance()();
    } while (r >= limit);

    std::uint_fast64_t bucket_index = r / bucket_size;

    assert(bucket_index < number_of_buckets);

    std::size_t result = min + bucket_index;

    assert(min <= result && result <= max);

    return result;
}

double uniform_min_max_integer_distribution(std::uint_fast64_t min, std::uint_fast64_t max, double x) {
    assert(min <= max);

    if (x < static_cast<double>(min)) {
        return 0.;
    }

    if (x >= static_cast<double>(max)) {
        return 1.;
    }

    return (std::floor(x - static_cast<double>(min)) + 1) / (static_cast<double>(max - min) + 1);
}

double uniform_zero_one_continuous_distribution(double x) {
    if (x < 0) {
        return 0;
    }

    if (x > 1) {
        return 1;
    }

    return x;
}

}  // namespace qx::random
