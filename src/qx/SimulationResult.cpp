#include "qx/Core.hpp"  // BasisVector
#include "qx/QuantumState.hpp"
#include "qx/SimulationResult.hpp"

#include <cstdint>  // uint8_t
#include <fmt/core.h>
#include <ostream>


namespace qx {

SimulationResult::SimulationResult(std::uint64_t requestedShots, std::uint64_t doneShots,
                                   RegisterManager const& registerManager)
    : shotsRequested{ requestedShots }
    , shotsDone{ doneShots }
    , qubitRegister{ registerManager.get_qubit_register() }
    , bitRegister{ registerManager.get_bit_register() }
{}

std::uint8_t SimulationResult::getQubitState(std::string const& stateString, std::string const& qubitVariableName,
                                             std::optional<Index> subIndex) {
    auto index = qubitRegister.at(qubitVariableName, subIndex);
    return static_cast<std::uint8_t>(stateString[stateString.size() - index - 1] - '0');
}

std::uint8_t SimulationResult::getBitMeasurement(std::string const& stateString, std::string const& bitVariableName,
                                                 std::optional<Index> subIndex) {
    auto index = bitRegister.at(bitVariableName, subIndex);
    return static_cast<std::uint8_t>(stateString[stateString.size() - index - 1] - '0');
}

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

SimulationResult SimulationResultAccumulator::getSimulationResult(RegisterManager const& registerManager) {
    assert(measurementsCount > 0);

    SimulationResult simulationResult{ measurementsCount, measurementsCount, registerManager };

    forAllNonZeroStates(
        [this, &simulationResult](core::BasisVector const& superposedState, core::SparseComplex const& sparseComplex) {
            auto stateString = superposedState.toSubstring(state.getNumberOfQubits());
            auto c = sparseComplex.value;
            auto amplitude = core::Complex{ .real = c.real(), .imag = c.imag(), .norm = std::norm(c) };
            simulationResult.state.push_back(SuperposedState{ stateString, amplitude });
        }
    );

    for (auto const& [stateString, count] : measurements) {
        simulationResult.measurements.push_back(Measurement{ stateString, count });
    }
    for (auto const& [stateString, count] : bitMeasurements) {
        simulationResult.bitRegisterMeasurements.push_back(Measurement{ stateString, count });
    }

    return simulationResult;
}

} // namespace qx