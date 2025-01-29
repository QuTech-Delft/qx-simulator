#include "qx/instructions.hpp"

#include "qx/random.hpp"

namespace qx {

BitControlledInstruction::BitControlledInstruction(ControlBits control_bits, std::shared_ptr<Instruction> instruction)
: control_bits{ std::move(control_bits) }
, instruction{ std::move(instruction) } {}

void BitControlledInstruction::execute(SimulationIterationContext& context) {
    auto is_bit_set = [&context](
                          const auto& control_bit) { return context.measurement_register.test(control_bit.value); };
    if (std::all_of(control_bits.begin(), control_bits.end(), is_bit_set)) {
        instruction->execute(context);
    }
}

[[nodiscard]] qubit_indices_t BitControlledInstruction::get_qubit_indices() {
    return instruction->get_qubit_indices();
}

[[nodiscard]] bit_indices_t BitControlledInstruction::get_bit_indices() {
    return instruction->get_bit_indices();
}

Unitary::Unitary(std::shared_ptr<core::matrix_t> matrix, std::shared_ptr<core::operands_t> operands)
: matrix{ std::move(matrix) }
, operands{ std::move(operands) } {}

void Unitary::execute(SimulationIterationContext& context) {
    context.state.apply(*matrix, *operands);
}

[[nodiscard]] std::shared_ptr<core::matrix_t> Unitary::inverse() const {
    return std::make_shared<core::matrix_t>(matrix->inverse());
}

[[nodiscard]] std::shared_ptr<core::matrix_t> Unitary::power(double exponent) const {
    return std::make_shared<core::matrix_t>(matrix->power(exponent));
}

[[nodiscard]] std::shared_ptr<core::matrix_t> Unitary::control() const {
    return std::make_shared<core::matrix_t>(matrix->control());
}

[[nodiscard]] qubit_indices_t Unitary::get_qubit_indices() {
    return *operands;
}

[[nodiscard]] bit_indices_t Unitary::get_bit_indices() {
    return bit_indices_t{};
}

[[nodiscard]] qubit_indices_t Measure::get_qubit_indices() {
    return qubit_indices_t{ qubit_index };
}

Measure::Measure(const core::QubitIndex& qubit_index, const core::BitIndex& bit_index)
: qubit_index{ qubit_index }
, bit_index{ bit_index } {}

void Measure::execute(SimulationIterationContext& context) {
    context.state.apply_measure(qubit_index,
        bit_index,
        &random::random_zero_one_double,
        context.measurement_register,
        context.bit_measurement_register);
}

[[nodiscard]] bit_indices_t Measure::get_bit_indices() {
    return bit_indices_t{ bit_index };
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

[[nodiscard]] qubit_indices_t Reset::get_qubit_indices() {
    return qubit_index.has_value() ? qubit_indices_t{ *qubit_index } : qubit_indices_t{};
}

[[nodiscard]] bit_indices_t Reset::get_bit_indices() {
    return bit_indices_t{};
}

}  // namespace qx
