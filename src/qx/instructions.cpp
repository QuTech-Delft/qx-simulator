#include "qx/instructions.hpp"

#include <algorithm>  // all_of

#include "qx/random.hpp"

namespace qx {

ControlledInstruction::ControlledInstruction(ControlBits control_bits, std::shared_ptr<Instruction> instruction)
: control_bits{ std::move(control_bits) }
, instruction{ std::move(instruction) } {}

void ControlledInstruction::execute(SimulationIterationContext& context) {
    auto is_bit_set = [&context](
                          auto const& control_bit) { return context.measurement_register.test(control_bit.value); };
    if (std::all_of(control_bits.begin(), control_bits.end(), is_bit_set)) {
        instruction->execute(context);
    }
}

Measure::Measure(core::QubitIndex const& qubit_index, core::BitIndex const& bit_index)
: qubit_index{ qubit_index }
, bit_index{ bit_index } {}

void Measure::execute(SimulationIterationContext& context) {
    context.state.apply_measure(qubit_index,
        bit_index,
        &random::random_zero_one_double,
        context.measurement_register,
        context.bit_measurement_register);
}

Reset::Reset(std::optional<core::QubitIndex> qubit_index)
: qubit_index{ qubit_index } {}

void Reset::execute(SimulationIterationContext& context) {
    if (qubit_index.has_value()) {
        context.state.apply_reset(qubit_index.value());
    } else {
        context.state.apply_reset_all();
    }
}

}  // namespace qx
