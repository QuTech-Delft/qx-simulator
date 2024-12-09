#pragma once

#include "qx/simulation_error.hpp"
#include "qx/simulation_result.hpp"

#include <optional>
#include <string>
#include <variant>  // monostate


namespace qx {

std::variant<std::monostate, SimulationResult, SimulationError> executeString(
    std::string const &s,
    std::size_t iterations = 1,
    std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string cqasm_version = "3.0");

std::variant<std::monostate, SimulationResult, SimulationError> executeFile(
    std::string const &filePath,
    std::size_t iterations = 1,
    std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string cqasm_version = "3.0");

}  // namespace qx
