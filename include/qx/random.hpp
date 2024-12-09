#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>


namespace qx::random {

void seed(std::uint_fast64_t seedValue);

double random_zero_one_double();

std::uint_fast64_t random_integer(std::uint_fast64_t min, std::uint_fast64_t max);

double uniform_min_max_integer_distribution(std::uint_fast64_t min, std::uint_fast64_t max, double x);

double uniform_zero_one_continuous_distribution(double x);

// TODO: add function for Bernoulli and use that in measure.

} // namespace qx::random
