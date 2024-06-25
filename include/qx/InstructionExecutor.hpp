#pragma once

#include "qx/Instructions.hpp"
#include "qx/QuantumState.hpp"

#include <cstddef>  // size_t


namespace qx{

class InstructionExecutor {
public:
    explicit InstructionExecutor(core::QuantumState &s);

    void operator()(Measure const &m);
    void operator()(MeasurementRegisterOperation const &op);

    template <std::size_t N>
    void operator()(Unitary<N> const &u) {
        quantumState.apply(u.matrix, u.operands);
    }

private:
    core::QuantumState &quantumState;
};

} // namespace qx
