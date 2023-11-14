#pragma once

#include "qx/Simulator.hpp"

namespace qxelarator {

std::variant<qx::SimulationResult, qx::SimulationError>
execute_string(std::string const &s, std::size_t iterations = 1,
               std::optional<std::uint_fast64_t> seed = std::nullopt) {
    return qx::executeString(s, iterations, seed);
}

std::variant<qx::SimulationResult, qx::SimulationError>
execute_file(std::string const &filePath, std::size_t iterations = 1,
             std::optional<std::uint_fast64_t> seed = std::nullopt) {
    return qx::executeFile(filePath, iterations, seed);
}

}  // namespace qxelarator
