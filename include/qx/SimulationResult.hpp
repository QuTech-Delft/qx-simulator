#pragma once

#include "qx/CompileTimeConfiguration.hpp"
#include "qx/Core.hpp"  // BasisVector, Complex
#include "qx/QuantumState.hpp"

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


struct SimulationResult {
    using Measurements = std::vector<Measurement>;
    using State = std::vector<SuperposedState>;

public:
    SimulationResult(std::uint64_t requestedShots, std::uint64_t doneShots);

public:
    std::uint64_t shotsRequested;
    std::uint64_t shotsDone;

    State state;
    Measurements measurements;
};

std::ostream &operator<<(std::ostream &os, const SimulationResult &result);


class SimulationResultAccumulator {
public:
    explicit SimulationResultAccumulator(core::QuantumState &s);
    void appendMeasurement(core::BasisVector const& measurement);
    SimulationResult getSimulationResult();

private:
    template <typename F>
    void forAllNonZeroStates(F &&f) {
        state.forEach([&f](auto const &kv) {
            f(kv.first, kv.second);
        });
    }

    core::QuantumState &state;
    absl::btree_map<state_string_t, count_t> measurements;
    std::uint64_t measurementsCount = 0;
};


}  // namespace qx

template <> struct fmt::formatter<qx::SimulationResult> : fmt::ostream_formatter {};
