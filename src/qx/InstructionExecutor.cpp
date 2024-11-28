#include "qx/InstructionExecutor.hpp"
#include "qx/Random.hpp"


namespace qx {

InstructionExecutor::InstructionExecutor(core::QuantumState &state, core::BasisVector &measurement_register)
    : state_{ state }
    , measurement_register_{ measurement_register }
{}

void InstructionExecutor::operator()(Measure const &m) {
    state_.measure(m.qubitIndex, &random::randomZeroOneDouble, measurement_register_);
}

void InstructionExecutor::operator()(MeasurementRegisterOperation const &op) {
    op.operation(measurement_register_);
}

}  // namespace qx
