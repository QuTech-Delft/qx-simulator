#pragma once

#include "qx/Common.hpp"

#include <absl/container/btree_map.h>
#include <complex>
#include <fmt/ostream.h>
#include <string>
#include <vector>


namespace qx {

namespace core {
class QuantumState;
}

struct Complex {
    double real = 0;
    double imag = 0;
    double norm = 0;
};

struct SimulationResult {
    using Results = std::vector<std::pair<std::string, std::uint64_t>>;

    using State = std::vector<std::pair<std::string, Complex>>;

    std::uint64_t shots_requested = 0;
    std::uint64_t shots_done = 0;

    Results results;
    State state;
};

std::ostream &operator<<(std::ostream &os, SimulationResult const &r);

class SimulationResultAccumulator {
public:
    explicit SimulationResultAccumulator(qx::core::QuantumState &s) : quantumState(s){};

    void append(BasisVector measuredState);

    SimulationResult get();

private:
    template <typename F> void forAllNonZeroStates(F &&f);

    std::string getStateString(BasisVector s);

    core::QuantumState &quantumState;
    absl::btree_map<BasisVector, std::uint64_t> measuredStates;
    std::uint64_t nMeasurements = 0;
};

}  // namespace qx

template <> struct fmt::formatter<qx::SimulationResult> : fmt::ostream_formatter {};
