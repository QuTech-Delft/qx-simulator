#pragma once

#include <complex>
#include <string>
#include <vector>

#include "absl/container/btree_map.h"
#include "qx/Common.hpp"


namespace qx {

namespace core {
class QuantumState;
}

struct SimulationResult {
    using Results = std::vector<std::pair<std::string, std::uint64_t>>;

    struct Complex {
        double real = 0;
        double imag = 0;
        double norm = 0;
    };

    using State = std::vector<std::pair<std::string, Complex>>;

    std::uint64_t shots_requested = 0;
    std::uint64_t shots_done = 0;

    Results results;
    State state;

    std::string getJsonString();
};

std::ostream &operator<<(std::ostream &os, SimulationResult const &r);

class SimulationResultAccumulator {
public:
    SimulationResultAccumulator(qx::core::QuantumState &s) : quantumState(s){};

    void append(BasisVector measuredState);

    void dump();

    SimulationResult get();

private:
    template <typename F> void forAllNonZeroStates(F &&f);

    std::string getStateString(BasisVector s);

    core::QuantumState &quantumState;
    absl::btree_map<BasisVector, std::uint64_t> measuredStates;
    std::uint64_t nMeasurements = 0;
};

}  // namespace qx