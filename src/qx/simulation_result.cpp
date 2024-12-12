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
                                   RegisterManager const& register_manager)
    : shots_requested{ requestedShots }
    , shots_done{ doneShots }
    , qubit_register{ register_manager.get_qubit_register() }
    , bit_register{ register_manager.get_bit_register() }
{}

std::uint8_t SimulationResult::get_qubit_state(state_string_t const& state_string, std::string const& qubit_variable_name,
                                               std::optional<Index> sub_index) {
    auto index = qubit_register.at(qubit_variable_name, sub_index);
    return static_cast<std::uint8_t>(state_string[state_string.size() - index - 1] - '0');
}

std::uint8_t SimulationResult::get_bit_measurement(state_string_t const& state_string, std::string const& bit_variable_name,
                                                   std::optional<Index> sub_index) {
    auto index = bit_register.at(bit_variable_name, sub_index);
    return static_cast<std::uint8_t>(state_string[state_string.size() - index - 1] - '0');
}

std::ostream &operator<<(std::ostream &os, const SimulationResult &simulation_result) {
    fmt::print(os, "State:\n");
    for (auto const &superposed_state : simulation_result.state) {
        fmt::print(os, "\t{1}  {2:.{0}f} + {3:.{0}f}i  (norm = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   superposed_state.value,
                   superposed_state.amplitude.real,
                   superposed_state.amplitude.imag,
                   superposed_state.amplitude.norm);
    }
    fmt::print(os, "Measurements:\n");
    for (auto const &measurement : simulation_result.measurements) {
        fmt::print(os, "\t{1}  {2}/{3}  (count/shots % = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   measurement.state,
                   measurement.count,
                   simulation_result.shots_done,
                   static_cast<double>(measurement.count) / static_cast<double>(simulation_result.shots_done));
    }
    fmt::print(os, "Bit measurements:\n");
    for (auto const &bit_measurement : simulation_result.bit_measurements) {
        fmt::print(os, "\t{1}  {2}/{3}  (count/shots % = {4:.{0}f})\n",
                   config::OUTPUT_DECIMALS,
                   bit_measurement.state,
                   bit_measurement.count,
                   simulation_result.shots_done,
                   static_cast<double>(bit_measurement.count) / static_cast<double>(simulation_result.shots_done));
    }
    fmt::print(os, "Qubit register:\n\t{}\n", simulation_result.qubit_register);
    fmt::print(os, "Bit register:\n\t{}", simulation_result.bit_register);
    return os;
}


//----------------------------//
// SimulationIterationContext //
//----------------------------//

SimulationIterationContext::SimulationIterationContext(RegisterManager const& register_manager)
    : state{ register_manager.get_qubit_register_size(), register_manager.get_bit_register_size() }
    , measurement_register{ register_manager.get_qubit_register_size() }
    , bit_measurement_register{ register_manager.get_bit_register_size() }
{}


//--------------------------------//
// SimulationIterationAccumulator //
//--------------------------------//

void SimulationIterationAccumulator::add(SimulationIterationContext const& context) {
    state = context.state;
    append_measurement(context.measurement_register);
    append_bit_measurement(context.bit_measurement_register);
}

void SimulationIterationAccumulator::append_measurement(core::BasisVector const& measurement) {
    assert(measurements.size() < (static_cast<size_t>(1) << state.get_number_of_qubits()));
    auto measured_state_string{ core::to_substring(measurement, state.get_number_of_qubits()) };
    measurements[measured_state_string]++;
    measurements_count++;
}

void SimulationIterationAccumulator::append_bit_measurement(core::BitMeasurementRegister const& bit_measurement) {
    assert(bit_measurements.size() < (static_cast<size_t>(1) << state.get_number_of_qubits()));
    auto bit_measured_state_string{ fmt::format("{}", bit_measurement) };
    bit_measurements[bit_measured_state_string]++;
    bit_measurements_count++;
}

SimulationResult SimulationIterationAccumulator::get_simulation_result(RegisterManager const& register_manager) {
    assert(measurements_count > 0);

    SimulationResult simulation_result{ measurements_count, measurements_count, register_manager };

    forAllNonZeroStates(
        [this, &simulation_result](core::BasisVector const& superposed_state, core::SparseComplex const& sparse_complex) {
            auto state_string = core::to_substring(superposed_state, state.get_number_of_qubits());
            auto c = sparse_complex.value;
            auto amplitude = amplitude_t{ c.real(), c.imag(), std::norm(c) };
            simulation_result.state.push_back(SuperposedState{ state_string, amplitude });
        }
    );

    for (auto const& [state_string, count] : measurements) {
        simulation_result.measurements.push_back(Measurement{ state_string, count });
    }
    for (auto const& [state_string, count] : bit_measurements) {
        simulation_result.bit_measurements.push_back(Measurement{ state_string, count });
    }

    return simulation_result;
}

} // namespace qx
