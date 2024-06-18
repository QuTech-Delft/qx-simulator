#pragma once

#include "qx/CompileTimeConfiguration.hpp"
#include "qx/Core.hpp"  // BasisVector, BitMeasurementRegister Complex
#include "qx/QuantumState.hpp"
#include "qx/RegisterManager.hpp"

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


struct Measurement {
    std::string state;
    std::uint64_t count;

    std::partial_ordering operator<=>(const Measurement &other) const = default;
};


struct SuperposedState {
    std::string value;
    core::Complex amplitude;

    bool operator==(const SuperposedState &other) const = default;
    std::partial_ordering operator<=>(const SuperposedState &other) const = default;
};


struct SimulationResult {
    using Measurements = std::vector<Measurement>;
    using State = std::vector<SuperposedState>;

public:
    SimulationResult(std::uint64_t requestedShots, std::uint64_t doneShots, RegisterManager const &registerManager);

    // Given a state string from the State vector, a qubit variable name, and an optional sub index,
    // return the value of that qubit in the state string
    // The sub index is used to access a given qubit when the qubit variable is of array type
    // Notice that the final index in the state string is determined by the qubit register
    std::uint8_t getQubitState(std::string const& stateString, std::string const& qubitVariableName,
                               std::optional<Index> subIndex);
    // Given a state string from the State vector, a bit variable name, and an optional sub index,
    // return the value of that bit in the state string
    // The sub index is used to access a given bit when the bit variable is of array type
    // Notice that the final index in the state string is determined by the bit register
    std::uint8_t getBitMeasurement(std::string const& stateString, std::string const& bitVariableName,
                                   std::optional<Index> subIndex);

public:
    std::uint64_t shotsRequested;
    std::uint64_t shotsDone;

    QubitRegister qubitRegister;
    BitRegister bitRegister;

    State state;
    Measurements measurements;
    Measurements bitRegisterMeasurements;
};

std::ostream &operator<<(std::ostream &os, const SimulationResult &result);


class SimulationResultAccumulator {
    using state_string_t = std::string;
    using count_t = std::uint64_t;

public:
    explicit SimulationResultAccumulator(core::QuantumState &s);
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

    core::QuantumState &state;

    // TODO: we are keeping a "double-entry bookkeeping" until we can get rid of measurements
    //   measurements needs to be replaced with bitMeasurements with the introduction of bit variables,
    //   but this replacement cannot be executed until all the QX simulator clients start using bitMeasurements
    absl::btree_map<state_string_t, count_t> measurements;
    absl::btree_map<state_string_t, count_t> bitMeasurements;

    std::uint64_t measurementsCount = 0;
    std::uint64_t bitMeasurementsCount = 0;
};


}  // namespace qx

template <> struct fmt::formatter<qx::SimulationResult> : fmt::ostream_formatter {};
