#pragma once

#include "cqasm.hpp"
#include "qx/SimulationResult.hpp"

#include <variant>
#include <string>


namespace qx {

struct SimulationError {
    std::string message = "Simulation error";
};

std::variant<SimulationResult, SimulationError>
executeString(std::string const &s, std::size_t iterations = 1,
              std::optional<std::uint_fast64_t> seed = std::nullopt);

std::variant<SimulationResult, SimulationError>
executeFile(std::string const &filePath, std::size_t iterations = 1,
            std::optional<std::uint_fast64_t> seed = std::nullopt);

}  // namespace qx
