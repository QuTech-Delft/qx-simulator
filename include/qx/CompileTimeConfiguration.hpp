#pragma once

#include <cstddef>  // size_t
#include <cstdint>  // uint64_t


namespace qx::config {

// Epsilon for double comparison
static constexpr double EPS = 0.000000000001;

// Number of decimals in output
static constexpr std::uint8_t const OUTPUT_DECIMALS = 8;

// How many gates between cleaning the zeros in the sparse array
static constexpr std::uint64_t ZERO_CYCLE_SIZE = 100;

// Maximum number of qubits that can be used.
// Maybe memory-saving as a multiple of 64.
static constexpr std::size_t MAX_QUBIT_NUMBER = 64;

}  // namespace qx::config
