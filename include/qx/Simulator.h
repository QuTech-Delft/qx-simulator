#pragma once

#include "cqasm.hpp"

#include <string>

namespace qx {

namespace core {
class QuantumState;
}

class Simulator {
public:
    Simulator();

    ~Simulator();

    void setJSONOutputPath(std::string filename) {
        jsonOutputFilename = filename;
    }

    bool set(std::string filePath);

    bool setString(std::string cqasmData);

    std::string execute(size_t numberOfRuns = 1);

private:
    std::string jsonOutputFilename = "";
    std::unique_ptr<qx::core::QuantumState> quantumState;
    cqasm::v1::ast::One<cqasm::v1::semantic::Program> program;
};

} // namespace qx