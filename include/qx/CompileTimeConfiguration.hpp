#pragma once

namespace qx::config {

// Epsilon for double comparison
static constexpr double EPS = 0.000000000001;

// Number of decimals in output
static constexpr std::uint64_t const OUTPUT_DECIMALS = 8;

// How many gates between cleaning the zeros in the sparse array
static constexpr std::uint64_t ZERO_CYCLE_SIZE = 100;

// Maximum number of qubits that can be used.
// Maybe memory-saving as a multiple of 64.
// In the future, make this a template parameter and change the data structures
// used based on the runtime number of qubits.
static constexpr std::size_t MAX_QUBIT_NUMBER = 64;

}  // namespace qx::config
