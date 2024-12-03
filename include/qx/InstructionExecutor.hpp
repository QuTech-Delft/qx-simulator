#pragma once

#include "qx/Core.hpp"
#include "qx/Instructions.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/QuantumState.hpp"
#include "qx/SimulationResult.hpp"

#include <cstddef>  // size_t


namespace qx{

class InstructionExecutor {
public:
    explicit InstructionExecutor(RegisterManager &register_manager);

    void operator()(Measure const &m);
    void operator()(Reset const &m);
    void operator()(ResetAll const &m);
    void operator()(MeasurementRegisterOperation const &op);

    template <std::size_t N>
    void operator()(Unitary<N> const &u) {
        simulation_iteration_result_.state.apply(u.matrix, u.operands);
    }

    SimulationIterationResult &getSimulationIterationResult();

private:
    SimulationIterationResult simulation_iteration_result_;
};

} // namespace qx
