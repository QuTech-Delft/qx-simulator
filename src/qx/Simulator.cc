#include "qx/Simulator.h"

#include "qx/Circuit.h"
#include "qx/LibqasmInterface.h"
#include "qx/SimulationResult.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace qx {

bool Simulator::set(std::string filePath) {
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

bool Simulator::setString(std::string cqasmData) {
    auto analyzer = cqasm::v1::default_analyzer("1.2");

    try {
        program = analyzer.analyze_string(cqasmData).unwrap();
    } catch (const cqasm::v1::analyzer::AnalysisFailed &e) {
        std::cerr << "Cannot parse and analyze string " << cqasmData
                  << std::endl;
        program.reset();
        return false;
    }

    if (program.empty()) {
        std::cerr << "Cannot parse and analyze string " << cqasmData
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

std::optional<SimulationResult> Simulator::execute(size_t numberOfRuns) {
    if (program.empty()) {
        std::cerr << "No circuit successfully loaded, call set(...) first"
                  << std::endl;
        return {};
    }

    if (numberOfRuns <= 0) {
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

    for (size_t s = 0; s < numberOfRuns; ++s) {
        quantumState.reset();
        for (auto &perfectCircuit : perfectCircuits) {
            perfectCircuit->execute(quantumState);
        }
        simulationResultAccumulator.append(
            quantumState.getMeasurementRegister());
    }

    auto simulationResult = simulationResultAccumulator.get();

    std::cout << simulationResult << std::endl;

    if (jsonOutputFilename != "") {
        auto resultJson = simulationResult.getJsonString();
        std::ofstream outfile(jsonOutputFilename);
        outfile << resultJson;
    }

    return simulationResult;
}

} // namespace qx