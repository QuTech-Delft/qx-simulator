#include "qx/core.hpp" // BasisVector
#include "qx/quantum_state.hpp"
#include "qx/simulation_result.hpp"

#include <cstdint>  // uint8_t
#include <fmt/core.h>
#include <ostream>


namespace qx {


//------------------//
// SimulationResult //
//------------------//

SimulationResult::SimulationResult(std::uint64_t requestedShots, std::uint64_t doneShots,
                                   RegisterManager const& registerManager)
    : shotsRequested{ requestedShots }
    , shotsDone{ doneShots }
    , qubitRegister{ registerManager.get_qubit_register() }
    , bitRegister{ registerManager.get_bit_register() }
{}

std::uint8_t SimulationResult::getQubitState(state_string_t const& stateString, std::string const& qubitVariableName,
                                             std::optional<Index> subIndex) {
    auto index = qubitRegister.at(qubitVariableName, subIndex);
    return static_cast<std::uint8_t>(stateString[stateString.size() - index - 1] - '0');
}

std::uint8_t SimulationResult::getBitMeasurement(state_string_t const& stateString, std::string const& bitVariableName,
                                                 std::optional<Index> subIndex) {
    auto index = bitRegister.at(bitVariableName, subIndex);
    return static_cast<std::uint8_t>(stateString[stateString.size() - index - 1] - '0');
}

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
    fmt::print(os, "Bit measurements:\n");
    for (auto const &bitMeasurement : simulationResult.bitMeasurements) {
        fmt::print(os, "\t{1}  {2}/{3}  (count/shots % = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   bitMeasurement.state,
                   bitMeasurement.count,
                   simulationResult.shotsDone,
                   static_cast<double>(bitMeasurement.count) / static_cast<double>(simulationResult.shotsDone));
    }
    fmt::print(os, "Qubit register:\n\t{}\n", simulationResult.qubitRegister);
    fmt::print(os, "Bit register:\n\t{}", simulationResult.bitRegister);
    return os;
}


//----------------------------//
// SimulationIterationContext //
//----------------------------//

SimulationIterationContext::SimulationIterationContext(RegisterManager const& registerManager)
    : state{ registerManager.get_qubit_register_size(), registerManager.get_bit_register_size() }
      , measurement_register{}
      , bit_measurement_register{ registerManager.get_bit_register_size() }
{}


//--------------------------------//
// SimulationIterationAccumulator //
//--------------------------------//

void SimulationIterationAccumulator::add(SimulationIterationContext const& context) {
    state = context.state;
    appendMeasurement(context.measurement_register);
    appendBitMeasurement(context.bit_measurement_register);
}

void SimulationIterationAccumulator::appendMeasurement(core::BasisVector const& measurement) {
    assert(measurements.size() < (static_cast<size_t>(1) << state.getNumberOfQubits()));
    auto measuredStateString{ measurement.toSubstring(state.getNumberOfQubits()) };
    measurements[measuredStateString]++;
    measurementsCount++;
}

void SimulationIterationAccumulator::appendBitMeasurement(core::BitMeasurementRegister const& bitMeasurement) {
    assert(bitMeasurements.size() < (static_cast<size_t>(1) << state.getNumberOfQubits()));
    auto bitMeasuredStateString{ fmt::format("{}", bitMeasurement) };
    bitMeasurements[bitMeasuredStateString]++;
    bitMeasurementsCount++;
}

SimulationResult SimulationIterationAccumulator::getSimulationResult(RegisterManager const& registerManager) {
    assert(measurementsCount > 0);

    SimulationResult simulationResult{ measurementsCount, measurementsCount, registerManager };

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
    for (auto const& [stateString, count] : bitMeasurements) {
        simulationResult.bitMeasurements.push_back(Measurement{ stateString, count });
    }

    return simulationResult;
}

} // namespace qx
