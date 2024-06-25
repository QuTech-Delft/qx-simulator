#include "qx/Core.hpp"  // BasisVector
#include "qx/QuantumState.hpp"
#include "qx/SimulationResult.hpp"

#include <cstdint>  // uint8_t
#include <fmt/core.h>
#include <ostream>


namespace qx {

SimulationResult::SimulationResult(std::uint64_t requestedShots, std::uint64_t doneShots)
    : shotsRequested{ requestedShots }
    , shotsDone{ doneShots }
{}

std::ostream &operator<<(std::ostream &os, const SimulationResult &simulationResult) {
    fmt::print(os, "State:\n");
    for (auto const &superposedState : simulationResult.state) {
        fmt::print(os, "\t{1}  {2:.{0}f} + {3:.{0}f}i  (norm = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   superposedState.value,
                   superposedState.amplitude.real,
                   superposedState.amplitude.imag,
                   superposedState.amplitude.norm);
    }
    fmt::print(os, "Measurements:\n");
    for (auto const &measurement : simulationResult.measurements) {
        fmt::print(os, "\t{1}  {2}/{3}  (count/shots % = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   measurement.state,
                   measurement.count,
                   simulationResult.shotsDone,
                   static_cast<double>(measurement.count) / static_cast<double>(simulationResult.shotsDone));
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

SimulationResult SimulationResultAccumulator::getSimulationResult() {
    assert(measurementsCount > 0);

    SimulationResult simulationResult{ measurementsCount, measurementsCount };

    forAllNonZeroStates(
        [this, &simulationResult](core::BasisVector const& superposedState, core::SparseComplex const& sparseComplex) {
            auto stateString = superposedState.toSubstring(state.getNumberOfQubits());
            auto c = sparseComplex.value;
            auto amplitude = amplitude_t{ c.real(), c.imag(), std::norm(c) };
            simulationResult.state.push_back(SuperposedState{ stateString, amplitude });
        }
    );

    for (auto const& [stateString, count] : measurements) {
        simulationResult.measurements.push_back(Measurement{ stateString, count });
    }

    return simulationResult;
}

} // namespace qx
