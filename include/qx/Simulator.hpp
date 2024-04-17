#pragma once

#include "qx/SimulationResult.hpp"

#include <optional>
#include <string>
#include <variant>


namespace qx {

struct SimulationError {
    std::string message = "Simulation error";
};

std::variant<SimulationResult, SimulationError>
executeString(
    std::string const &s,
    std::size_t iterations = 1,
    std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string cqasm_version = "3.0");

std::variant<SimulationResult, SimulationError>
executeFile(
    std::string const &filePath,
    std::size_t iterations = 1,
    std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string cqasm_version = "3.0");

}  // namespace qx
