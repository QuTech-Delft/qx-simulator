#pragma once

#include "qx/compile_time_configuration.hpp"
#include "qx/core.hpp" // BasisVector, BitMeasurementRegister Complex
#include "qx/quantum_state.hpp"
#include "qx/register_manager.hpp"

#include <absl/container/btree_map.h>
#include <compare>  // partial_ordering
#include <complex>
#include <cstdint>  // uint64_t
#include <fmt/ostream.h>
#include <ostream>
#include <string>
#include <vector>


namespace qx {


namespace core {
class QuantumState;
}


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

    std::partial_ordering operator<=>(const Measurement &other) const = default;
};


struct SuperposedState {
    state_string_t value;
    amplitude_t amplitude;

    bool operator==(const SuperposedState &other) const = default;
    std::partial_ordering operator<=>(const SuperposedState &other) const = default;
};


//------------------//
// SimulationResult //
//------------------//

struct SimulationResult {
    using Measurements = std::vector<Measurement>;
    using State = std::vector<SuperposedState>;

public:
    SimulationResult(std::uint64_t requestedShots, std::uint64_t doneShots, RegisterManager const& registerManager);

    // Given a state string from the State vector, a qubit variable name, and an optional sub index,
    // return the value of that qubit in the state string
    // The sub index is used to access a given qubit when the qubit variable is of array type
    // Notice that the final index in the state string is determined by the qubit register
    std::uint8_t getQubitState(state_string_t const& stateString, std::string const& qubitVariableName,
                               std::optional<Index> subIndex);
    // Given a state string from the State vector, a bit variable name, and an optional sub index,
    // return the value of that bit in the state string
    // The sub index is used to access a given bit when the bit variable is of array type
    // Notice that the final index in the state string is determined by the bit register
    std::uint8_t getBitMeasurement(state_string_t const& stateString, std::string const& bitVariableName,
                                   std::optional<Index> subIndex);

public:
    std::uint64_t shotsRequested;
    std::uint64_t shotsDone;

    QubitRegister qubitRegister;
    BitRegister bitRegister;

    State state;
    Measurements measurements;
    Measurements bitMeasurements;
};

std::ostream &operator<<(std::ostream &os, const SimulationResult &result);


//----------------------------//
// SimulationIterationContext //
//----------------------------//

struct SimulationIterationContext {
    core::QuantumState state;
    core::BasisVector measurement_register;
    core::BitMeasurementRegister bit_measurement_register;

    explicit SimulationIterationContext(RegisterManager const& registerManager);
};


//--------------------------------//
// SimulationIterationAccumulator //
//--------------------------------//

class SimulationIterationAccumulator {
public:
    void add(const SimulationIterationContext &simulationIterationContext);
    void appendMeasurement(core::BasisVector const& measurement);
    void appendBitMeasurement(core::BitMeasurementRegister const& bitMeasurement);
    SimulationResult getSimulationResult(RegisterManager const& registerManager);

private:
    template <typename F>
    void forAllNonZeroStates(F &&f) {
        state.forEach([&f](auto const &kv) {
            f(kv.first, kv.second);
        });
    }

    core::QuantumState state;
    absl::btree_map<state_string_t, count_t> measurements;
    absl::btree_map<state_string_t, count_t> bitMeasurements;

    std::uint64_t measurementsCount = 0;
    std::uint64_t bitMeasurementsCount = 0;
};


}  // namespace qx

template <> struct fmt::formatter<qx::SimulationResult> : fmt::ostream_formatter {};
