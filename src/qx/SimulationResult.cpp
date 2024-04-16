#include "qx/SimulationResult.hpp"

#include "qx/Core.hpp"
#include <iomanip>
#include <iostream>
#include <variant>


namespace qx {

void SimulationResultAccumulator::append(BasisVector measuredState) {
    assert(measuredStates.size() <= (1u << quantumState.getNumberOfQubits()));
    measuredStates[measuredState]++;
    nMeasurements++;
}

std::ostream &operator<<(std::ostream &os, SimulationResult const &r) {
    os << std::setprecision(config::OUTPUT_DECIMALS) << std::fixed;
    os << "-------------------------------------------" << std::endl;

    os << "Final quantum state" << std::endl;

    for (auto const &kv : r.state) {
        auto const &stateString = kv.first;
        auto const &amplitude = kv.second;
        os << stateString << "       " << amplitude.real << " + "
           << amplitude.imag << "*i   "
           << " (p = " << amplitude.norm << ")" << std::endl;
    }

    os << std::endl << "Measurement register averaging" << std::endl;

    for (const auto &kv : r.results) {
        const auto &stateString = kv.first;
        const auto &count = kv.second;
        os << stateString << "       " << count << "/" << r.shots_done << " ("
           << static_cast<double>(count) / static_cast<double>(r.shots_done) << ")" << std::endl;
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
    quantumState.forEach(
        [&f, this](auto const &kv) { f(getStateString(kv.first), kv.second); });
}

std::string SimulationResultAccumulator::getStateString(BasisVector s) {
    auto str = s.toString();

    return str.substr(str.size() - quantumState.getNumberOfQubits(),
                      str.size());
}

} // namespace qx