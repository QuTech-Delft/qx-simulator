#pragma once

#include "qx/simulator.hpp"

#include <variant>  // monostate


namespace qxelarator {

std::variant<std::monostate, qx::SimulationResult, qx::SimulationError> execute_string(
    std::string const &s,
    std::size_t iterations = 1,
    std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string version = "3.0") {

    return qx::execute_string(s, iterations, seed, std::move(version));
}

std::variant<std::monostate, qx::SimulationResult, qx::SimulationError> execute_file(
    std::string const &file_path,
    std::size_t iterations = 1,
    std::optional<std::uint_fast64_t> seed = std::nullopt,
    std::string version = "3.0") {

    return qx::execute_file(file_path, iterations, seed, std::move(version));
}

}  // namespace qxelarator
