#pragma once

#include <variant>  // monostate

#include "qx/simulator.hpp"

namespace qxelarator {

/**
 * Simulates a cQASM program from a string.
 *
 * Parameters:
 *   - `program` is a string containing the cQASM program.
 *   - `iterations` is the number of times the program is being simulated.
 *   - `seed` can be used in case a deterministic output is needed.
 *   - `version` is the cQASM program version.
 *
 * Returns either a simulation result or an error.
 */
inline std::variant<std::monostate, qx::SimulationResult, qx::SimulationError> execute_string(
    const std::string& program, std::size_t iterations = 1, std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string version = "3.0") {
    return qx::execute_string(program, iterations, seed, std::move(version));
}

/**
 * Simulates a cQASM program from a file.
 *
 * Parameters:
 *   - `file_path` is the file path of the cQASM program.
 *   - `iterations` is the number of times the program is being simulated.
 *   - `seed` can be used in case a deterministic output is needed.
 *   - `version` is the cQASM program version.
 *
 * Returns either a simulation result or an error.
 */
inline std::variant<std::monostate, qx::SimulationResult, qx::SimulationError> execute_file(
    const std::string& file_path, std::size_t iterations = 1, std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string version = "3.0") {
    return qx::execute_file(file_path, iterations, seed, std::move(version));
}

}  // namespace qxelarator
