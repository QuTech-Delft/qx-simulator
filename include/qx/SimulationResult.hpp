#pragma once

#include "qx/CompileTimeConfiguration.hpp"
#include "qx/Core.hpp"  // Complex

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

struct Result {
    std::string state;
    std::uint64_t count;
    std::partial_ordering operator<=>(const Result &other) const = default;
};

struct State {
    std::string value;
    core::Complex amplitude;
    bool operator==(const State &other) const = default;
    std::partial_ordering operator<=>(const State &other) const = default;
};

struct SimulationResult {
    using Results = std::vector<Result>;
    using States = std::vector<State>;
    std::uint64_t shots_requested = 0;
    std::uint64_t shots_done = 0;
    Results results;
    States states;
};

std::ostream &operator<<(std::ostream &os, const SimulationResult &result);

using state_t = core::BasisVector;
using count_t = std::uint64_t;

class SimulationResultAccumulator {
public:
    explicit SimulationResultAccumulator(core::QuantumState &s);
    void append(core::BasisVector measuredState);
    SimulationResult get();

private:
    template <typename F>
    void forAllNonZeroStates(F &&f);
    std::string getStateString(core::BasisVector s);

    core::QuantumState &quantumState;
    absl::btree_map<state_t, count_t> measuredStates;
    std::uint64_t nMeasurements = 0;
};

}  // namespace qx

template <> struct fmt::formatter<qx::SimulationResult> : fmt::ostream_formatter {};
