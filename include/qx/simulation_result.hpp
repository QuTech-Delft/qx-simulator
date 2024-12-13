#pragma once

#include <fmt/ostream.h>

#include <cstdint>  // uint64_t
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "qx/compile_time_configuration.hpp"
#include "qx/core.hpp"
#include "qx/quantum_state.hpp"
#include "qx/register_manager.hpp"

namespace qx {

// A simulation consists of a number of iterations, each executing a circuit composed by a list of instructions.
//
// Each instruction is executed within a given SimulationIterationContext:
// - a quantum state, and
// - some measurement registers.
// The SimulationIterationContext is reset at the beginning of every iteration,
// returned at the end of the iteration as a simulation iteration result,
// and accumulated by a SimulationIterationAccumulator.
//
// The final SimulationResult is composed by the SimulationIterationAccumulator at the end of a simulation:
// - the quantum state is just the state of the last iteration.
// - the measurement registers contain all the measurements for all the iterations.

using state_string_t = std::string;
using count_t = std::uint64_t;
using amplitude_t = core::Complex;

struct Measurement {
    state_string_t state;
    count_t count;

    std::partial_ordering operator<=>(const Measurement& other) const = default;
};

struct SuperposedState {
    state_string_t value;
    amplitude_t amplitude;

    bool operator==(const SuperposedState& other) const = default;
    std::partial_ordering operator<=>(const SuperposedState& other) const = default;
};

//------------------//
// SimulationResult //
//------------------//

struct SimulationResult {
    using Measurements = std::vector<Measurement>;
    using State = std::vector<SuperposedState>;

public:
    SimulationResult(std::uint64_t shots_done, std::uint64_t shots_requested,
        std::shared_ptr<QubitRegister> qubit_register, std::shared_ptr<BitRegister> bit_register);

    // Given a state string from the State vector, a qubit variable name, and an optional sub index,
    // return the value of that qubit in the state string
    // The sub index is used to access a given qubit when the qubit variable is of array type
    // Notice that the final index in the state string is determined by the qubit register
    std::uint8_t get_qubit_state(
        state_string_t const& state_string, std::string const& qubit_variable_name, std::optional<Index> sub_index);
    // Given a state string from the State vector, a bit variable name, and an optional sub index,
    // return the value of that bit in the state string
    // The sub index is used to access a given bit when the bit variable is of array type
    // Notice that the final index in the state string is determined by the bit register
    std::uint8_t get_bit_measurement(
        state_string_t const& state_string, std::string const& bit_variable_name, std::optional<Index> sub_index);

public:
    std::uint64_t shots_requested;
    std::uint64_t shots_done;

    QubitRegister qubit_register;
    BitRegister bit_register;

    State state;
    Measurements measurements;
    Measurements bit_measurements;
};

std::ostream& operator<<(std::ostream& os, const SimulationResult& result);

//----------------------------//
// SimulationIterationContext //
//----------------------------//

struct SimulationIterationContext {
    core::QuantumState state;
    core::MeasurementRegister measurement_register;
    core::BitMeasurementRegister bit_measurement_register;

    explicit SimulationIterationContext();
};

//--------------------------------//
// SimulationIterationAccumulator //
//--------------------------------//

class SimulationIterationAccumulator {
public:
    void add(const SimulationIterationContext& context);
    void append_measurement(core::MeasurementRegister const& measurement);
    void append_bit_measurement(core::BitMeasurementRegister const& bit_measurement);
    SimulationResult get_simulation_result();

private:
    template <typename F>
    void for_all_non_zero_states(F&& f) {
        state.for_each([&f](auto const& kv) { f(kv.first, kv.second); });
    }

    core::QuantumState state;
    std::map<state_string_t, count_t> measurements;
    std::map<state_string_t, count_t> bit_measurements;

    std::uint64_t measurements_count = 0;
    std::uint64_t bit_measurements_count = 0;
};

}  // namespace qx

template <>
struct fmt::formatter<qx::SimulationResult> : fmt::ostream_formatter {};
