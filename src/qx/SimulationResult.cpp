#include "qx/SimulationResult.hpp"

#include "qx/Core.hpp"  // BasisVector
#include "qx/QuantumState.hpp"
#include <fmt/core.h>
#include <ostream>


namespace qx {

std::ostream &operator<<(std::ostream &os, const SimulationResult &simulationResult) {
    fmt::print("\nFinal quantum state\n");
    for (auto const &superposedState : simulationResult.state) {
        fmt::print("{1}  {2:.{0}f} + {3:.{0}f}i  (norm = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   superposedState.value,
                   superposedState.amplitude.real,
                   superposedState.amplitude.imag,
                   superposedState.amplitude.norm);
    }
    fmt::print("\nMeasurement register averaging\n");
    for (auto const &result : simulationResult.measurements) {
        fmt::print("{1}  {2}/{3}  (count/shots % = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   result.state,
                   result.count,
                   simulationResult.shotsDone,
                   static_cast<double>(result.count) / static_cast<double>(simulationResult.shotsDone));
    }
    return os;
}

SimulationResultAccumulator::SimulationResultAccumulator(core::QuantumState &s)
    : state(s)
{}

void SimulationResultAccumulator::appendMeasurement(core::BasisVector const& measurement) {
    assert(measurements.size() < (static_cast<size_t>(1) << state.getNumberOfQubits()));
    auto measuredStateString{ measurement.toSubstring(state.getNumberOfQubits()) };
    measurements[measuredStateString]++;
    measurementsCount++;
}

void SimulationResultAccumulator::appendBitMeasurement(core::BitMeasurementRegister const& bitMeasurement) {
    assert(bitMeasurements.size() < (static_cast<size_t>(1) << state.getNumberOfQubits()));
    auto bitMeasuredStateString{ fmt::format("{}", bitMeasurement) };
    bitMeasurements[bitMeasuredStateString]++;
    bitMeasurementsCount++;
}

SimulationResult SimulationResultAccumulator::getSimulationResult(const RegisterManager &registerManager) {
    assert(measurementsCount > 0);

    SimulationResult simulationResult;

    simulationResult.shotsRequested = measurementsCount;
    simulationResult.shotsDone = measurementsCount;

    forAllNonZeroStates([this, &simulationResult](core::BasisVector const& superposedState, core::SparseComplex const& sparseComplex) {
        auto stateString = superposedState.toSubstring(state.getNumberOfQubits());
        auto c = sparseComplex.value;
        auto amplitude = core::Complex{ .real = c.real(), .imag = c.imag(), .norm = std::norm(c) };
        simulationResult.state.push_back(SuperposedState{ stateString, amplitude });
    });

    for (auto const& [stateString, count] : measurements) {
        simulationResult.measurements.push_back(Measurement{ stateString, count });
    }

    for (auto const& [stateString, count] : bitMeasurements) {
        simulationResult.bitRegisterMeasurements.push_back(Measurement{ stateString, count });
    }

    simulationResult.qubitRegister = registerManager.get_qubit_register().getVariableNameToRangeMap();
    simulationResult.bitRegister = registerManager.get_bit_register().getVariableNameToRangeMap();

    return simulationResult;
}

} // namespace qx