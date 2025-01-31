#pragma once

#include <cstddef>  // size_t
#include <cstdint>  // uint64_t

namespace qx::config {

// Epsilon for double comparison
static constexpr double EPSILON = 0.000'000'000'000'1;

// Number of decimals in output
static constexpr const std::uint8_t OUTPUT_DECIMALS = 8;

// How many gates between cleaning the zeros in the sparse array
static constexpr std::uint64_t ZERO_CYCLE_SIZE = 100;

// Maximum number of qubits that can be used.
// Maybe memory-saving as a multiple of 64.
static constexpr std::size_t MAX_QUBIT_NUMBER = 64;

// Maximum number of bits that can be used.
// Just for sanity, as we maintain vectors of the size of the number of used bits.
static constexpr std::size_t MAX_BIT_NUMBER = 1 * 1024 * 1024;  // 1 MB

}  // namespace qx::config
