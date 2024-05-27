#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>


namespace qx::random {

void seed(std::uint_fast64_t seedValue);

double randomZeroOneDouble();

std::uint_fast64_t randomInteger(std::uint_fast64_t min,
                                 std::uint_fast64_t max);

double uniformMinMaxIntegerDistribution(std::uint_fast64_t min,
                                        std::uint_fast64_t max, double x);

double uniformZeroOneContinuousDistribution(double x);

// TODO: add function for Bernoulli and use that in measure.

} // namespace qx::random
