#include "qx/InstructionExecutor.hpp"
#include "qx/Random.hpp"


namespace qx {

InstructionExecutor::InstructionExecutor(core::QuantumState &s)
    : quantumState(s)
{}

void InstructionExecutor::operator()(Measure const &m) {
    quantumState.applyMeasure(m.qubitIndex, &random::randomZeroOneDouble);
}

void InstructionExecutor::operator()(Reset const &r) {
    quantumState.applyReset(r.qubitIndex);
}

void InstructionExecutor::operator()(ResetAll const &/* r */) { quantumState.applyResetAll();
}

void InstructionExecutor::operator()(MeasurementRegisterOperation const &op) {
    op.operation(quantumState.getMeasurementRegister());
}

}  // namespace qx
