#pragma once

#include "cqasm.hpp"
#include "qx/SimulationResult.h"

#include <optional>
#include <string>

namespace qx {

namespace core {
class QuantumState;
}

class Simulator {
public:
    void setJSONOutputPath(std::string filename) {
        jsonOutputFilename = filename;
    }

    bool set(std::string filePath);

    bool setString(std::string cqasmData);

    std::optional<SimulationResult> execute(size_t numberOfRuns = 1);

private:
    std::string jsonOutputFilename = "";
    cqasm::v1::ast::One<cqasm::v1::semantic::Program> program;
};

} // namespace qx