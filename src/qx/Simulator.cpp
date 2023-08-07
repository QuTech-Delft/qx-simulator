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

bool Simulator::set(std::string const &filePath) {
    auto analyzer = cqasm::v1x::default_analyzer("1.2");

    try {
        program = analyzer.analyze(filePath).unwrap();
    } catch (const cqasm::v1x::analyzer::AnalysisFailed &e) {
        std::cerr << "Cannot parse and analyze file " << filePath << std::endl;
        program.reset();
        return false;
    }

    if (program.empty()) {
        std::cerr << "Cannot parse and analyze file " << filePath << std::endl;
        return false;
    }

    if (!program->error_model.empty() &&
        program->error_model->name != "depolarizing_channel") {
        std::cerr << "Unknown error model: " << program->error_model->name
                  << std::endl;
        program.reset();
        return false;
    }

    return true;
}

bool Simulator::setString(std::string const &s) {
    auto analyzer = cqasm::v1x::default_analyzer("1.2");

    try {
        program = analyzer.analyze_string(s).unwrap();
    } catch (const cqasm::v1x::analyzer::AnalysisFailed &e) {
        std::cerr << "Cannot parse and analyze string " << s << std::endl;
        program.reset();
        return false;
    }

    if (program.empty()) {
        std::cerr << "Cannot parse and analyze string " << s << std::endl;
        return false;
    }

    if (!program->error_model.empty() &&
        program->error_model->name != "depolarizing_channel") {
        std::cerr << "Unknown error model: " << program->error_model->name
                  << std::endl;
        program.reset();
        return false;
    }

    return true;
}

std::optional<SimulationResult>
Simulator::execute(std::size_t iterations,
                   std::optional<std::uint_fast64_t> seed) const {
    if (program.empty()) {
        std::cerr << "No circuit successfully loaded, call set(...) first"
                  << std::endl;
        return {};
    }

    if (iterations <= 0) {
        std::cerr << "Number of runs is 0" << std::endl;
        return {};
    }

    if (seed) {
        random::seed(*seed);
    }

    std::size_t qubitCount = program->num_qubits;
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

std::optional<SimulationResult>
executeString(std::string const &s, std::size_t iterations,
              std::optional<std::uint_fast64_t> seed) {
    Simulator simulator;
    if (!simulator.setString(s)) {
        return {};
    }

    return simulator.execute(iterations, seed);
}

std::optional<SimulationResult>
executeFile(std::string const &filePath, std::size_t iterations,
            std::optional<std::uint_fast64_t> seed) {
    Simulator simulator;
    if (!simulator.set(filePath)) {
        return {};
    }

    return simulator.execute(iterations, seed);
}

} // namespace qx
