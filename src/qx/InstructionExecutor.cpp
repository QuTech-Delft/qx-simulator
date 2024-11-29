#include "qx/InstructionExecutor.hpp"
#include "qx/Random.hpp"


namespace qx {

InstructionExecutor::InstructionExecutor(core::QuantumState &state, core::BasisVector &measurement_register,
                                         core::BitMeasurementRegister &bit_measurement_register)
    : state_{ state }
    , measurement_register_{ measurement_register }
    , bit_measurement_register_{ bit_measurement_register }
{}

void InstructionExecutor::operator()(Measure const &m) {
    state_.applyMeasure(m.qubitIndex, m.bitIndex, &random::randomZeroOneDouble, measurement_register_,
                        bit_measurement_register_);
}

void InstructionExecutor::operator()(Reset const &r) {
    state_.applyReset(r.qubitIndex);
}

void InstructionExecutor::operator()(ResetAll const &/* r */) {
    state_.applyResetAll();
}

void InstructionExecutor::operator()(MeasurementRegisterOperation const &op) {
    op.operation(measurement_register_);
}

}  // namespace qx
