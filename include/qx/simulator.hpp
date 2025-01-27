#pragma once

#include <optional>
#include <string>
#include <variant>  // monostate

#include "qx/simulation_error.hpp"
#include "qx/simulation_result.hpp"

namespace qx {

std::variant<std::monostate, SimulationResult, SimulationError> execute_string(const std::string& s,
    std::size_t iterations = 1, std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string cqasm_version = "3.0");

std::variant<std::monostate, SimulationResult, SimulationError> execute_file(const std::string& file_path,
    std::size_t iterations = 1, std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string cqasm_version = "3.0");

}  // namespace qx
