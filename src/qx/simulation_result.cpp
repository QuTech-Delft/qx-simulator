#include "qx/simulation_result.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <cstdint>  // uint8_t
#include <ostream>

#include "qx/core.hpp"
#include "qx/quantum_state.hpp"
#include "qx/register_manager.hpp"

namespace qx {

//------------------//
// SimulationResult //
//------------------//

SimulationResult::SimulationResult(std::uint64_t shots_done, std::uint64_t shots_requested,
    std::shared_ptr<QubitRegister> qubit_register, std::shared_ptr<BitRegister> bit_register)
: shots_requested{ shots_requested }
, shots_done{ shots_done }
, qubit_register{ *qubit_register }
, bit_register{ *bit_register } {}

std::uint8_t SimulationResult::get_qubit_state(
    const state_string_t& state_string, const std::string& qubit_variable_name, std::optional<Index> sub_index) {
    auto index = qubit_register.at(qubit_variable_name, sub_index);
    return static_cast<std::uint8_t>(state_string[state_string.size() - index - 1] - '0');
}

std::uint8_t SimulationResult::get_bit_measurement(
    const state_string_t& state_string, const std::string& bit_variable_name, std::optional<Index> sub_index) {
    auto index = bit_register.at(bit_variable_name, sub_index);
    return static_cast<std::uint8_t>(state_string[state_string.size() - index - 1] - '0');
}

std::ostream& operator<<(std::ostream& os, const Measurement& measurement) {
    return os << (measurement.state.empty()
                   ? std::string{}
                   : fmt::format("state='{}', count='{}'", measurement.state, measurement.count));
}

std::ostream& operator<<(std::ostream& os, const SuperposedState& superposed_state) {
    return os << fmt::format("value='{1}', amplitude='{2:.{0}f} + {3:.{0}f}i', norm='{4:.{0}f}'",
               config::OUTPUT_DECIMALS,
               superposed_state.value,
               superposed_state.amplitude.real,
               superposed_state.amplitude.imag,
               superposed_state.amplitude.norm);
}

std::ostream& operator<<(std::ostream& os, const SimulationResult& simulation_result) {
    fmt::print(os, "Shots requested: {}\n", simulation_result.shots_requested);
    fmt::print(os, "Shots done: {}\n", simulation_result.shots_done);
    fmt::print(os, "State:\n\t{}\n", fmt::join(simulation_result.state, "\n\t"));
    fmt::print(os, "Measurements:\n\t{}\n", fmt::join(simulation_result.measurements, "\n\t"));
    fmt::print(os, "Bit measurements:\n\t{}\n", fmt::join(simulation_result.bit_measurements, "\n\t"));
    fmt::print(os, "Qubit register:\n\t{}\n", simulation_result.qubit_register);
    fmt::print(os, "Bit register:\n\t{}", simulation_result.bit_register);
    return os;
}

//----------------------------//
// SimulationIterationContext //
//----------------------------//

SimulationIterationContext::SimulationIterationContext()
: state{ RegisterManager::get_instance().get_qubit_register_size(),
    RegisterManager::get_instance().get_bit_register_size() }
, measurement_register{ RegisterManager::get_instance().get_qubit_register_size() }
, bit_measurement_register{ RegisterManager::get_instance().get_bit_register_size() } {}

//--------------------------------//
// SimulationIterationAccumulator //
//--------------------------------//

void SimulationIterationAccumulator::add(const SimulationIterationContext& context) {
    state = context.state;
    // Notice that the simulator always stores the values of the measurement registers
    // even when no measure instruction has been executed
    // When a measure instruction is executed, the outcome of the quantum state collapse is simulated
    append_measurement(context.measurement_register);
    append_bit_measurement(context.bit_measurement_register);
    shots_done++;
}

void SimulationIterationAccumulator::append_measurement(const core::MeasurementRegister& measurement) {
    assert(measurements.size() < (static_cast<size_t>(1) << state.get_number_of_qubits()));
    auto measured_state_string{ core::to_substring(measurement, state.get_number_of_qubits()) };
    measurements[measured_state_string]++;
}

void SimulationIterationAccumulator::append_bit_measurement(const core::BitMeasurementRegister& bit_measurement) {
    assert(bit_measurements.size() < (static_cast<size_t>(1) << state.get_number_of_qubits()));
    auto bit_measured_state_string{ fmt::format("{}", bit_measurement) };
    bit_measurements[bit_measured_state_string]++;
}

// Notice that, with the current implementation:
// - Either the number of requested shots is equal the number of done shots.
// - Or a simulation error has occurred.
SimulationResult SimulationIterationAccumulator::get_simulation_result(std::size_t shots_requested) {
    assert(shots_done > 0);

    SimulationResult simulation_result{ shots_done,
        shots_requested,
        RegisterManager::get_instance().get_qubit_register(),
        RegisterManager::get_instance().get_bit_register() };

    for_all_non_zero_states([this, &simulation_result](
                                const core::BasisVector& superposed_state, const core::SparseComplex& sparse_complex) {
        auto state_string = core::to_substring(superposed_state, state.get_number_of_qubits());
        auto c = sparse_complex.value;
        auto amplitude = amplitude_t{ c.real(), c.imag(), std::norm(c) };
        simulation_result.state.push_back(SuperposedState{ state_string, amplitude });
    });

    for (const auto& [state_string, count] : measurements) {
        simulation_result.measurements.push_back(Measurement{ state_string, count });
    }
    for (const auto& [state_string, count] : bit_measurements) {
        simulation_result.bit_measurements.push_back(Measurement{ state_string, count });
    }

    return simulation_result;
}

}  // namespace qx
