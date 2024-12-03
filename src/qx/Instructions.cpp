#include "qx/Instructions.hpp"
#include "qx/Random.hpp"

#include <algorithm>  // all_of


namespace qx {


ControlledInstruction::ControlledInstruction(ControlBits const& control_bits, std::shared_ptr<Instruction> instruction)
    : control_bits{ control_bits }
    , instruction{ instruction }
{}


void ControlledInstruction::execute(SimulationContext &context) {
    auto is_bit_set = [&context](auto const &control_bit) {
        return context.measurement_register.test(control_bit.value);
    };
    if (std::all_of(control_bits.begin(), control_bits.end(), is_bit_set)) {
        instruction->execute(context);
    }
}


Measure::Measure(core::QubitIndex const& qubitIndex, core::BitIndex const& bitIndex)
    : qubitIndex{ qubitIndex }
    , bitIndex{ bitIndex }
{}


void Measure::execute(SimulationContext &context) {
    context.state.applyMeasure(
        qubitIndex,
        bitIndex,
        &random::randomZeroOneDouble,
        context.measurement_register,
        context.bit_measurement_register
    );
}


Reset::Reset(std::optional<core::QubitIndex> qubitIndex)
    : qubitIndex{ std::move(qubitIndex) }
{}


void Reset::execute(SimulationContext &context) {
    if (qubitIndex.has_value()) {
        context.state.applyReset(qubitIndex.value());
    } else {
        context.state.applyResetAll();
    }
}


}  // namespace qx
