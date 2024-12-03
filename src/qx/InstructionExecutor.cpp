#include "qx/InstructionExecutor.hpp"
#include "qx/Random.hpp"


namespace qx {

InstructionExecutor::InstructionExecutor(RegisterManager &register_manager)
    : simulation_iteration_result_{ register_manager }
{}

void InstructionExecutor::operator()(Measure const &m) {
    simulation_iteration_result_.state.applyMeasure(
        m.qubitIndex,
        m.bitIndex,
        &random::randomZeroOneDouble,
        simulation_iteration_result_.measurement_register,
        simulation_iteration_result_.bit_measurement_register);
}

void InstructionExecutor::operator()(Reset const &r) {
    simulation_iteration_result_.state.applyReset(r.qubitIndex);
}

void InstructionExecutor::operator()(ResetAll const &/* r */) {
    simulation_iteration_result_.state.applyResetAll();
}

void InstructionExecutor::operator()(MeasurementRegisterOperation const &op) {
    op.operation(simulation_iteration_result_.measurement_register);
}

SimulationIterationResult &InstructionExecutor::getSimulationIterationResult() {
    return simulation_iteration_result_;
}

}  // namespace qx
