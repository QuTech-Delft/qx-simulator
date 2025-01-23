#include "qx/instructions.hpp"

#include "qx/random.hpp"

namespace qx {

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
