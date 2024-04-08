#include "qx/Simulator.hpp"

#include "qx/Circuit.hpp"
#include "qx/ErrorModels.hpp"
#include "qx/V3xLibqasmInterface.hpp"
#include "qx/Random.hpp"
#include "qx/SimulationResult.hpp"

#include "v3x/cqasm.hpp"

#include <fstream>
#include <iostream>
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

    return analyzer.analyze_string(s);
}

std::variant<V1Program, SimulationError> getV1ProgramOrError(V1AnalysisResult const& analysisResult) {
    if (!analysisResult.errors.empty()) {
        std::stringstream error;
        error << "Cannot parse and analyze cQASM v1: \n";

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

std::variant<V3Program, SimulationError> getV3ProgramOrError(V3AnalysisResult const& analysisResult) {
    if (!analysisResult.errors.empty()) {
        std::stringstream error;
        error << "Cannot parse and analyze cQASM v3: \n";

        std::for_each(analysisResult.errors.begin(), analysisResult.errors.end(), [&error](auto const& x) { error << x << "\n"; });

        return SimulationError{error.str()};
    }

    auto program = analysisResult.root;
    assert(!program.empty());
    
    return program;
}

std::variant<SimulationResult, SimulationError>
execute(V1AnalysisResult analysisResult,
                std::size_t iterations,
                std::optional<std::uint_fast64_t> seed) {
    auto programOrError = getV1ProgramOrError(analysisResult);

    if (auto* error = std::get_if<SimulationError>(&programOrError)) {
        return *error;
    }

    auto program = std::get<V1Program>(programOrError);

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

std::variant<SimulationResult, SimulationError>
execute(V3AnalysisResult analysisResult,
                std::size_t iterations,
                std::optional<std::uint_fast64_t> seed) {
    auto programOrError = getV3ProgramOrError(analysisResult);

    // FIXME: assert that there is only one qubit register... and only one bit register with same size?

    if (auto* error = std::get_if<SimulationError>(&programOrError)) {
        return *error;
    }

    auto program = std::get<V3Program>(programOrError);

    assert(!program.empty());

    if (iterations <= 0) {
        return SimulationError{"Invalid number of iterations"};
    }

    if (seed) {
        random::seed(*seed);
    }

    std::size_t qubitCount = 0;
    for (auto const& v: program->variables) {
        if (v->typ->type() == cqasm::v3x::types::NodeType::QubitArray) {
            qubitCount += v->typ->as_qubit_array()->size;
        } else if (v->typ->type() == cqasm::v3x::types::NodeType::Qubit) {
            qubitCount += 1;
        }
    }

    if (qubitCount > config::MAX_QUBIT_NUMBER) {
        return SimulationError{"Cannot run that many qubits in this version of QX-simulator"};
    }

    // TODO: make mapping between simulator global qubit index and qubit register index.


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
executeString(std::string const &s, std::size_t iterations,
              std::optional<std::uint_fast64_t> seed, std::string cqasm_version) {    
    if (cqasm_version == "3.0") {
        auto analysisResult = parseCqasmV3xString(s);
        return execute(analysisResult, iterations, seed);
    } else {
        return SimulationError{"Unknown cqasm version: " + cqasm_version};
    }
}

std::variant<SimulationResult, SimulationError>
executeFile(std::string const &filePath, std::size_t iterations,
            std::optional<std::uint_fast64_t> seed, std::string cqasm_version) {
    if (cqasm_version == "3.0") {
        auto analysisResult = parseCqasmV3xFile(filePath);
        return execute(analysisResult, iterations, seed);
    } else {
        return SimulationError{"Unknown cqasm version: " + cqasm_version};
    }
}

} // namespace qx
