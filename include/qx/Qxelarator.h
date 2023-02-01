#pragma once

#include "qx/Simulator.h"

namespace qxelarator {

// Old API (deprecated).

class QX {
public:
    void set_json_output_path(std::string const& filePath) {
        simulator.setJSONOutputPath(filePath);
    }

    bool set(std::string filePath) {
        return simulator.set(filePath);
    }

    bool set_string(std::string s) {
        return simulator.setString(s);
    }

    std::string execute(std::size_t iterations = 1) {
        auto simulationResult = simulator.execute(iterations);
        assert(simulationResult);
        return simulationResult->getJsonString();
    }

private:
    qx::Simulator simulator;
};

// New API.

std::optional<qx::SimulationResult> execute_string(std::string const& s, std::size_t iterations = 1) {
    return qx::executeString(s, iterations);
}

std::optional<qx::SimulationResult> execute_file(std::string const& filePath, std::size_t iterations = 1) {
    return qx::executeFile(filePath, iterations);
}

} // namespace qxelarator