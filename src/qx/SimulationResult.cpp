#include "qx/SimulationResult.hpp"

#include "qx/Core.hpp"  // BasisVector
#include "qx/QuantumState.hpp"
#include <fmt/core.h>
#include <variant>


namespace qx {

void SimulationResultAccumulator::append(core::BasisVector measuredState) {
    assert(measuredStates.size() <= (1u << quantumState.getNumberOfQubits()));
    measuredStates[measuredState]++;
    nMeasurements++;
}

std::ostream &operator<<(std::ostream &os, SimulationResult const &r) {
    fmt::print("-------------------------------------------\n");
    fmt::print("Final quantum state\n");
    for (auto const &kv : r.state) {
        auto const &stateString = kv.first;
        auto const &amplitude = kv.second;
        fmt::print("{1}       {2:.{0}f} + {3:.{0}f}*i   (p = {4:.{0}f})\n", config::OUTPUT_DECIMALS, stateString,
                   amplitude.real, amplitude.imag, amplitude.norm);
    }
    fmt::print("\nMeasurement register averaging\n");
    for (const auto &kv : r.results) {
        const auto &stateString = kv.first;
        const auto &count = kv.second;
        fmt::print("{1}       {2}/{3} ({4:.{0}f})\n", config::OUTPUT_DECIMALS, stateString, count,
                   r.shots_done, static_cast<double>(count) / static_cast<double>(r.shots_done));
    }
    return os;
}

SimulationResult SimulationResultAccumulator::get() {
    SimulationResult simulationResult;
    simulationResult.shots_requested = nMeasurements;
    simulationResult.shots_done = nMeasurements;

    assert(nMeasurements > 0);
    for (const auto &kv : measuredStates) {
        const auto &state = kv.first;
        const auto &count = kv.second;
        simulationResult.results.emplace_back(getStateString(state), count);
    }
    forAllNonZeroStates([&simulationResult](auto stateString, auto c) {
        simulationResult.state.push_back(std::make_pair(
            stateString, Complex{ .real = c.real(), .imag = c.imag(), .norm = std::norm(c) }));
    });
    return simulationResult;
}

template <typename F>
void SimulationResultAccumulator::forAllNonZeroStates(F &&f) {
    quantumState.forEach([&f, this](auto const &kv) { f(getStateString(kv.first), kv.second); });
}

std::string SimulationResultAccumulator::getStateString(qx::core::BasisVector s) {
    auto str = s.toString();
    return str.substr(str.size() - quantumState.getNumberOfQubits(), str.size());
}

} // namespace qx