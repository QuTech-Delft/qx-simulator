#include "qx/Simulator.hpp"

#include "qx/Circuit.hpp"
#include "qx/ErrorModels.hpp"
#include "qx/LibqasmInterface.hpp"
#include "qx/Random.hpp"
#include "qx/SimulationResult.hpp"

#include <fstream>
#include <iostream>
#include <vector>


namespace qx {

using AnalysisResult = cqasm::v1x::analyzer::AnalysisResult;
using Program = cqasm::v1x::ast::One<cqasm::v1x::semantic::Program>;

namespace {
AnalysisResult parseCQASMV1File(std::string const &filePath) {
    auto analyzer = cqasm::v1x::default_analyzer("1.2");

    return analyzer.analyze(filePath);
}

AnalysisResult parseCQASMV1String(std::string const &s) {
    auto analyzer = cqasm::v1x::default_analyzer("1.2");

    return analyzer.analyze_string(s);
}

std::variant<Program, SimulationError> getProgramOrError(AnalysisResult const& analysisResult) {
    if (!analysisResult.errors.empty()) {
        std::stringstream error;
        error << "Cannot parse and analyze cQASM: \n";

        std::for_each(analysisResult.errors.begin(), analysisResult.errors.end(), [&error](auto const& x) { error << x << "\n"; });

        return SimulationError{error.str()};
    }

    auto program = analysisResult.root;
    assert(!program.empty());

    if (!program->error_model.empty() &&
        program->error_model->name != "depolarizing_channel") {
        std::cerr << "Unknown error model: " << program->error_model->name
                << std::endl;
        return SimulationError{"Unknown error model: " + program->error_model->name};
    }

    return program;
}
    
std::variant<SimulationResult, SimulationError>
execute(AnalysisResult analysisResult,
                std::size_t iterations,
                std::optional<std::uint_fast64_t> seed) {
    auto programOrError = getProgramOrError(analysisResult);

    if (auto* error = std::get_if<SimulationError>(&programOrError)) {
        return *error;
    }

    auto program = std::get<Program>(programOrError);

    assert(!program.empty());

    if (iterations <= 0) {
        return SimulationError{"Invalid number of iterations"};
    }

    if (seed) {
        random::seed(*seed);
    }

    std::size_t qubitCount = program->num_qubits;

    if (qubitCount > config::MAX_QUBIT_NUMBER) {
        return SimulationError{"Cannot run that many qubits in this version of QX-simulator"};
    }

    std::vector<qx::Circuit> perfectCircuits;

    qx::core::QuantumState quantumState(qubitCount);

    auto const &subcircuits = program->subcircuits;
    for (auto const &subcircuit : subcircuits) {
        perfectCircuits.push_back(loadCqasmCode(*subcircuit));
    }

    auto errorModel = error_models::getErrorModel(program->error_model);

    SimulationResultAccumulator simulationResultAccumulator(quantumState);

    for (std::size_t s = 0; s < iterations; ++s) {
        quantumState.reset();
        for (auto &perfectCircuit : perfectCircuits) {
            perfectCircuit.execute(quantumState, errorModel);
        }
        simulationResultAccumulator.append(
            quantumState.getMeasurementRegister());
    }

    auto simulationResult = simulationResultAccumulator.get();

    return simulationResult;
}
}

std::variant<SimulationResult, SimulationError>
executeString(std::string const &s, std::size_t iterations,
              std::optional<std::uint_fast64_t> seed) {
    auto analysisResult = parseCQASMV1String(s);

    return execute(analysisResult, iterations, seed);
}

std::variant<SimulationResult, SimulationError>
executeFile(std::string const &filePath, std::size_t iterations,
            std::optional<std::uint_fast64_t> seed) {
    auto analysisResult = parseCQASMV1File(filePath);

    return execute(analysisResult, iterations, seed);
}

} // namespace qx
