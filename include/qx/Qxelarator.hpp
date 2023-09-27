#pragma once

#include "qx/Simulator.hpp"

namespace qxelarator {

// Old API (deprecated).

class QX {
public:
    void set_json_output_path(std::string const &filePath) {
        simulator.setJSONOutputPath(filePath);
    }

    std::optional<qx::SimulationError> set(const std::string &filePath) { return simulator.set(filePath); }

    std::optional<qx::SimulationError> set_string(const std::string &s) { return simulator.setString(s); }

    std::string execute(std::size_t iterations = 1) {
        auto result = simulator.execute(iterations);
        if (auto* simulationError = std::get_if<qx::SimulationError>(&result)) {
            return simulationError->message;
        }

        return std::get_if<qx::SimulationResult>(&result)->getJsonString();
    }

private:
    qx::Simulator simulator;
};

// New API.

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
