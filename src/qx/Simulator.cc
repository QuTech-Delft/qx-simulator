#include "qx/Simulator.h"

#include "qx/Circuit.h"
#include "qx/LibqasmInterface.h"
#include "qx/SimulationResult.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace qx {

bool Simulator::set(std::string const& filePath) {
    auto analyzer = cqasm::v1::default_analyzer("1.2");

    try {
        program = analyzer.analyze(filePath).unwrap();
    } catch (const cqasm::v1::analyzer::AnalysisFailed &e) {
        std::cerr << "Cannot parse and analyze file " << filePath << std::endl;
        program.reset();
        return false;
    }

    if (program.empty()) {
        std::cerr << "Cannot parse and analyze file " << filePath << std::endl;
        return false;
    }

    if (!program->error_model.empty()) {
        std::cerr << "Error models not implemented yet" << std::endl;
        program.reset();
        return false;
    }

    return true;
}

bool Simulator::setString(std::string const& s) {
    auto analyzer = cqasm::v1::default_analyzer("1.2");

    try {
        program = analyzer.analyze_string(s).unwrap();
    } catch (const cqasm::v1::analyzer::AnalysisFailed &e) {
        std::cerr << "Cannot parse and analyze string " << s
                  << std::endl;
        program.reset();
        return false;
    }

    if (program.empty()) {
        std::cerr << "Cannot parse and analyze string " << s
                  << std::endl;
        return false;
    }

    if (!program->error_model.empty()) {
        std::cerr << "Error models not implemented yet" << std::endl;
        program.reset();
        return false;
    }

    return true;
}

std::optional<SimulationResult> Simulator::execute(std::size_t iterations) const {
    if (program.empty()) {
        std::cerr << "No circuit successfully loaded, call set(...) first"
                  << std::endl;
        return {};
    }

    if (iterations <= 0) {
        std::cerr << "Number of runs is 0" << std::endl;
        return {};
    }

    std::size_t qubitCount = program->num_qubits;
    std::vector<std::shared_ptr<qx::Circuit>> perfectCircuits;

    qx::core::QuantumState quantumState(qubitCount);

    auto const &subcircuits = program->subcircuits;
    for (auto const &subcircuit : subcircuits) {
        perfectCircuits.push_back(loadCqasmCode(*subcircuit));
    }

    SimulationResultAccumulator simulationResultAccumulator(quantumState);

    for (std::size_t s = 0; s < iterations; ++s) {
        quantumState.reset();
        for (auto &perfectCircuit : perfectCircuits) {
            perfectCircuit->execute(quantumState);
        }
        simulationResultAccumulator.append(
            quantumState.getMeasurementRegister());
    }

    auto simulationResult = simulationResultAccumulator.get();

    if (jsonOutputFilePath != "") {
        auto resultJson = simulationResult.getJsonString();
        std::ofstream outfile(jsonOutputFilePath);
        outfile << resultJson;
    }

    return simulationResult;
}

std::optional<SimulationResult> executeString(std::string const& s, std::size_t iterations) {
    Simulator simulator;
    if (!simulator.setString(s)) {
        return {};
    }

    return simulator.execute(iterations);
};

std::optional<SimulationResult> executeFile(std::string const& filePath, std::size_t iterations) {
    Simulator simulator;
    if (!simulator.set(filePath)) {
        return {};
    }

    return simulator.execute(iterations);
};

} // namespace qx