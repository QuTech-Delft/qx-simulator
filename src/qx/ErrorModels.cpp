#include "qx/ErrorModels.hpp"

#include "qx/Gates.hpp"
#include "qx/Random.hpp"


namespace qx::error_models {

void DepolarizingChannel::addError(qx::core::QuantumState &quantumState) const {
    auto random = random::randomZeroOneDouble();
    if (random > probability) {
        return;
    }

    assert(quantumState.getNumberOfQubits() > 0);
    std::array<core::QubitIndex, 1> operand{core::QubitIndex{
        random::randomInteger(0, quantumState.getNumberOfQubits() - 1)}};

    if (random < probability / 3) {
        quantumState.apply(gates::X, operand);
    } else if (random < 2 * probability / 3) {
        quantumState.apply(gates::Y, operand);
    } else {
        quantumState.apply(gates::Z, operand);
    }
}

} // namespace qx::error_models
