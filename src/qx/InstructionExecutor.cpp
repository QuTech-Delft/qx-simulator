#include "qx/InstructionExecutor.hpp"
#include "qx/Random.hpp"


namespace qx {

InstructionExecutor::InstructionExecutor(core::QuantumState &s)
    : quantumState(s)
{}

void InstructionExecutor::operator()(Measure const &m) {
    quantumState.apply_measure(m.qubitIndex, &random::randomZeroOneDouble);
}

void InstructionExecutor::operator()(Reset const &r) {
    quantumState.apply_reset(r.qubitIndex, &random::randomZeroOneDouble);
}

void InstructionExecutor::operator()(ResetAll const &/* r */) {
    quantumState.apply_reset_all();
}

void InstructionExecutor::operator()(MeasurementRegisterOperation const &op) {
    op.operation(quantumState.getMeasurementRegister());
}

}  // namespace qx
