#include "qx/InstructionExecutor.hpp"
#include "qx/Random.hpp"


namespace qx {

InstructionExecutor::InstructionExecutor(core::QuantumState &s)
    : quantumState(s)
{}

void InstructionExecutor::operator()(Measure const &m) {
    quantumState.measure(m.qubitIndex, m.bitIndex, &random::randomZeroOneDouble);
}

void InstructionExecutor::operator()(MeasurementRegisterOperation const &op) {
    op.operation(quantumState.getMeasurementRegister());
}

}  // namespace qx
