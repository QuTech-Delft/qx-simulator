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

std::optional<SimulationError> Simulator::set(std::string const &filePath) {
    auto analyzer = cqasm::v1x::default_analyzer("1.2");

    auto analysisResult = analyzer.analyze(filePath);
    if (!analysisResult.errors.empty()) {
        program.reset();
        std::stringstream error;
        error << "Cannot parse and analyze file " << filePath << ":";

        std::for_each(analysisResult.errors.begin(), analysisResult.errors.end(), [&error](auto const& s) { error << "\n" << s; });

        return SimulationError{error.str()};
    }

    program = analysisResult.root;
    assert(!program.empty());

    if (!program->error_model.empty() &&
        program->error_model->name != "depolarizing_channel") {
        program.reset();
        return SimulationError{"Unknown error model: " + program->error_model->name};
    }

    return std::nullopt;
}

std::optional<SimulationError> Simulator::setString(std::string const &s) {
    auto analyzer = cqasm::v1x::default_analyzer("1.2");

    auto analysisResult = analyzer.analyze_string(s);
    if (!analysisResult.errors.empty()) {
        program.reset();
        std::stringstream error;
        error << "Cannot parse and analyze string " << s << ": \n";

        std::for_each(analysisResult.errors.begin(), analysisResult.errors.end(), [&error](auto const& x) { error << x << "\n"; });

        return SimulationError{error.str()};
    }

    program = analysisResult.root;
    assert(!program.empty());

    if (!program->error_model.empty() &&
        program->error_model->name != "depolarizing_channel") {
        std::cerr << "Unknown error model: " << program->error_model->name
                  << std::endl;
        program.reset();
        return SimulationError{"Unknown error model: " + program->error_model->name};
    }

    return std::nullopt;
}

std::variant<SimulationResult, SimulationError>
Simulator::execute(std::size_t iterations,
                   std::optional<std::uint_fast64_t> seed) const {
    if (program.empty()) {
        return SimulationError{"No circuit successfully loaded, call set(...) first"};
    }

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

    if (!jsonOutputFilePath.empty()) {
        auto resultJson = simulationResult.getJsonString();
        std::ofstream outfile(jsonOutputFilePath);
        outfile << resultJson;
    }

    return simulationResult;
}

std::variant<SimulationResult, SimulationError>
executeString(std::string const &s, std::size_t iterations,
              std::optional<std::uint_fast64_t> seed) {
    Simulator simulator;
    if (auto error = simulator.setString(s)) {
        return *error;
    }

    return simulator.execute(iterations, seed);
}

std::variant<SimulationResult, SimulationError>
executeFile(std::string const &filePath, std::size_t iterations,
            std::optional<std::uint_fast64_t> seed) {
    Simulator simulator;
    if (auto error = simulator.set(filePath)) {
        return *error;
    }

    return simulator.execute(iterations, seed);
}

} // namespace qx
