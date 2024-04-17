#include "qx/Simulator.hpp"

#include "qx/Circuit.hpp"
#include "qx/ErrorModels.hpp"
#include "qx/V3xLibqasmInterface.hpp"
#include "qx/Random.hpp"
#include "qx/SimulationResult.hpp"

#include "v3x/cqasm.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>


namespace qx {

using V3AnalysisResult = cqasm::v3x::analyzer::AnalysisResult;
using V3Program = cqasm::v3x::ast::One<cqasm::v3x::semantic::Program>;

namespace {
V3AnalysisResult parseCqasmV3xFile(std::string const &filePath) {
    auto analyzer = cqasm::v3x::default_analyzer("3.0");

    return analyzer.analyze_file(filePath);
}

V3AnalysisResult parseCqasmV3xString(std::string const &s) {
    auto analyzer = cqasm::v3x::default_analyzer("3.0");

    return analyzer.analyze_string(s, std::nullopt);
}

std::variant<V3Program, SimulationError> getV3ProgramOrError(V3AnalysisResult const& analysisResult) {
    if (!analysisResult.errors.empty()) {
        auto error = fmt::format("Cannot parse and analyze cQASM v3:\n{}", fmt::join(analysisResult.errors, "\n"));
        return SimulationError{ error };
    }

    auto program = analysisResult.root;
    assert(!program.empty());
    
    return program;
}

std::variant<SimulationResult, SimulationError>
execute(
    V3AnalysisResult const& analysisResult,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed) {

    auto programOrError = getV3ProgramOrError(analysisResult);

    if (auto* error = std::get_if<SimulationError>(&programOrError)) {
        return *error;
    }

    auto program = std::get<V3Program>(programOrError);

    assert(!program.empty());

    if (iterations <= 0) {
        return SimulationError{ "Invalid number of iterations" };
    }

    if (seed) {
        random::seed(*seed);
    }

    std::size_t qubitCount = 0;
    auto const& v = program->qubit_variable_declaration;
    if (v->typ->type() == cqasm::v3x::types::NodeType::QubitArray) {
        qubitCount = v->typ->as_qubit_array()->size;
    } else if (v->typ->type() == cqasm::v3x::types::NodeType::Qubit) {
        qubitCount = 1;
    }

    if (qubitCount > config::MAX_QUBIT_NUMBER) {
        return SimulationError{ "Cannot run that many qubits in this version of QX-simulator" };
    }

    qx::core::QuantumState quantumState(qubitCount);

    qx::Circuit circuit = loadCqasmCode(*program);

    SimulationResultAccumulator simulationResultAccumulator(quantumState);

    for (std::size_t s = 0; s < iterations; ++s) {
        quantumState.reset();
        circuit.execute(quantumState, std::monostate{});
        simulationResultAccumulator.append(
            quantumState.getMeasurementRegister());
    }

    auto simulationResult = simulationResultAccumulator.get();

    return simulationResult;
}
}

std::variant<SimulationResult, SimulationError>
executeString(
    std::string const &s,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed,
    std::string cqasm_version) {

    if (cqasm_version == "3.0") {
        auto analysisResult = parseCqasmV3xString(s);
        return execute(analysisResult, iterations, seed);
    } else {
        return SimulationError{ fmt::format("Unknown cqasm version: {}", cqasm_version) };
    }
}

std::variant<SimulationResult, SimulationError>
executeFile(
    std::string const &filePath,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed,
    std::string cqasm_version) {

    if (cqasm_version == "3.0") {
        auto analysisResult = parseCqasmV3xFile(filePath);
        return execute(analysisResult, iterations, seed);
    } else {
        return SimulationError{ fmt::format("Unknown cqasm version: {}", cqasm_version) };
    }
}

} // namespace qx
