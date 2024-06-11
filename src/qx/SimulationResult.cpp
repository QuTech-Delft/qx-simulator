#include "qx/SimulationResult.hpp"

#include "qx/Core.hpp"  // BasisVector
#include "qx/QuantumState.hpp"
#include <fmt/core.h>
#include <ostream>


namespace qx {

std::ostream &operator<<(std::ostream &os, const SimulationResult &simulationResult) {
    fmt::print("\nFinal quantum state\n");
    for (auto const &state : simulationResult.states) {
        fmt::print("{1}  {2:.{0}f} + {3:.{0}f}i  (norm = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   state.value,
                   state.amplitude.real,
                   state.amplitude.imag,
                   state.amplitude.norm);
    }
    fmt::print("\nMeasurement register averaging\n");
    for (const auto &result : simulationResult.results) {
        fmt::print("{1}  {2}/{3}  (count/shots % = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   result.state,
                   result.count,
                   simulationResult.shots_done,
                   static_cast<double>(result.count) / static_cast<double>(simulationResult.shots_done));
    }
    return os;
}

SimulationResultAccumulator::SimulationResultAccumulator(core::QuantumState &s)
    : quantumState(s)
{}

void SimulationResultAccumulator::append(core::BasisVector measuredState) {
    assert(measuredStates.size() <= (static_cast<size_t>(1) << quantumState.getNumberOfQubits()));
    measuredStates[measuredState]++;
    nMeasurements++;
}

SimulationResult SimulationResultAccumulator::get() {
    SimulationResult simulationResult;
    simulationResult.shots_requested = nMeasurements;
    simulationResult.shots_done = nMeasurements;

    assert(nMeasurements > 0);
    for (const auto &[state, count] : measuredStates) {
        simulationResult.results.push_back(Result{ getStateString(state), count});
    }
    forAllNonZeroStates([&simulationResult](auto stateString, auto sparseComplex) {
        auto c = sparseComplex.value;
        simulationResult.states.push_back(
            State{ stateString, core::Complex{ .real = c.real(), .imag = c.imag(), .norm = std::norm(c) } });
    });
    return simulationResult;
}

template <typename F>
void SimulationResultAccumulator::forAllNonZeroStates(F &&f) {
    quantumState.forEach([&f, this](auto const &kv) {
        f(getStateString(kv.first), kv.second);
    });
}

std::string SimulationResultAccumulator::getStateString(qx::core::BasisVector s) {
    auto str = s.toString();
    return str.substr(str.size() - quantumState.getNumberOfQubits(), str.size());
}

} // namespace qx