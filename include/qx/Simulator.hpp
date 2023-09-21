#pragma once

#include "cqasm.hpp"
#include "qx/SimulationResult.hpp"

#include <variant>
#include <string>


namespace qx {

struct SimulationError {
    std::string message = "Simulation error";
};

// Old API (deprecated).
class Simulator {
public:
    void setJSONOutputPath(std::string_view filePath) {
        jsonOutputFilePath = filePath;
    }

    std::optional<SimulationError> set(std::string const &filePath);

    std::optional<SimulationError> setString(std::string const &s);

    [[nodiscard]] std::variant<SimulationResult, SimulationError>
    execute(std::size_t iterations = 1,
            std::optional<std::uint_fast64_t> seed = std::nullopt) const;

private:
    std::string jsonOutputFilePath;
    cqasm::v1x::ast::One<cqasm::v1x::semantic::Program> program;
};

// New API.
std::variant<SimulationResult, SimulationError>
executeString(std::string const &s, std::size_t iterations = 1,
              std::optional<std::uint_fast64_t> seed = std::nullopt);

std::variant<SimulationResult, SimulationError>
executeFile(std::string const &filePath, std::size_t iterations = 1,
            std::optional<std::uint_fast64_t> seed = std::nullopt);

}  // namespace qx
