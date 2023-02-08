#pragma once

#include "cqasm.hpp"
#include "qx/SimulationResult.h"

#include <optional>
#include <string>

namespace qx {

// Old API (deprecated).
class Simulator {
public:
    void setJSONOutputPath(std::string_view filePath) {
        jsonOutputFilePath = filePath;
    }

    bool set(std::string const& filePath);

    bool setString(std::string const& s);

    std::optional<SimulationResult> execute(std::size_t iterations = 1) const;

private:
    std::string jsonOutputFilePath = "";
    cqasm::v1::ast::One<cqasm::v1::semantic::Program> program;
};

// New API.
std::optional<SimulationResult> executeString(std::string const& s, std::size_t iterations = 1);

std::optional<SimulationResult> executeFile(std::string const& filePath, std::size_t iterations = 1);

} // namespace qx