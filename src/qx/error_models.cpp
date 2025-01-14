#include "qx/error_models.hpp"

#include "qx/gates.hpp"
#include "qx/random.hpp"

namespace qx::error_models {

DepolarizingChannel::DepolarizingChannel(double p)
: probability{ p } {
    assert(0. <= p && p <= 1.);
}

void DepolarizingChannel::add_error(qx::core::QuantumState& quantum_state) const {
    if (auto random = random::random_zero_one_double(); random <= probability) {
        assert(quantum_state.get_number_of_qubits() > 0);

        auto operand = std::array<core::QubitIndex, 1>{ core::QubitIndex{
            random::random_integer(0, quantum_state.get_number_of_qubits() - 1) } };

        if (random < probability / 3) {
            quantum_state.apply(gates::X, operand);
        } else if (random < 2 * probability / 3) {
            quantum_state.apply(gates::Y, operand);
        } else {
            quantum_state.apply(gates::Z, operand);
        }
    }
}

}  // namespace qx::error_models
