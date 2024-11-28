#pragma once

#include "qx/Core.hpp"
#include "qx/Instructions.hpp"
#include "qx/QuantumState.hpp"

#include <cstddef>  // size_t


namespace qx{

class InstructionExecutor {
public:
    explicit InstructionExecutor(core::QuantumState &state, core::BasisVector &measurement_register);

    void operator()(Measure const &m);
    void operator()(MeasurementRegisterOperation const &op);

    template <std::size_t N>
    void operator()(Unitary<N> const &u) {
        state_.apply(u.matrix, u.operands);
    }

private:
    core::QuantumState &state_;
    core::BasisVector &measurement_register_;
};

} // namespace qx
