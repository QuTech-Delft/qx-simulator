#pragma once

#include <complex>
#include <string>
#include <vector>

#include "absl/container/btree_map.h"
#include "qx/Common.h"

namespace qx {

namespace core {
class QuantumState;
}

struct SimulationResult {
    struct Complex {
        double real = 0;
        double imag = 0;
    };

    std::uint64_t shots_requested = 0;
    std::uint64_t shots_done = 0;

    std::vector<std::pair<std::string, double>> results;
    std::vector<std::pair<std::string, Complex>> state;

    std::string getJsonString();
};

class SimulationResultAccumulator {
public:
    SimulationResultAccumulator(qx::core::QuantumState &s) : quantumState(s){};

    void append(BasisVector measured_state) {
        measuredStates[measured_state]++;
        n_measurements++;
    }

    void dump();

    SimulationResult get();

private:
    template <typename F> void forAllNonZeroStates(F &&f);

    std::string get_state_string(BasisVector s);

    core::QuantumState &quantumState;
    absl::btree_map<BasisVector, std::uint64_t> measuredStates;
    std::uint64_t n_measurements = 0;
};

} // namespace qx