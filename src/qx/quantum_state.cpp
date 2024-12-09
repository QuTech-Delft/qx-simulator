#include "qx/quantum_state.hpp"

#include <complex>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <ostream>


namespace qx::core {

void QuantumState::check_quantum_state() {
    if (number_of_qubits_ == 0) {
        throw QuantumStateError{ "number of qubits needs to be at least 1" };
    }
    if (number_of_qubits_ > config::MAX_QUBIT_NUMBER) {
        throw QuantumStateError{ fmt::format("number of qubits exceeds maximum allowed: {} > {}", number_of_qubits_,
                                             config::MAX_QUBIT_NUMBER) };
    }
    if (number_of_bits_ > config::MAX_BIT_NUMBER) {
        throw QuantumStateError{ fmt::format("number of bits exceeds maximum allowed: {} > {}", number_of_bits_,
                                             config::MAX_BIT_NUMBER) };
    }
    if (not is_normalized()) {
        throw QuantumStateError{ "quantum state is not normalized" };
    }
}

QuantumState::QuantumState()
    : QuantumState{ 1, 1 }
{}

QuantumState::QuantumState(std::size_t qubit_register_size, std::size_t bit_register_size)
    : number_of_qubits_{ qubit_register_size }
    , number_of_bits_{ bit_register_size }
    , data_{ static_cast<size_t>(1) << number_of_qubits_ } {

    reset_data();
    check_quantum_state();
}

QuantumState::QuantumState(std::size_t qubit_register_size, std::size_t bit_register_size,
                           std::initializer_list<PairBasisVectorStringComplex> values)
    : number_of_qubits_{ qubit_register_size }
    , number_of_bits_{ bit_register_size }
    , data_{ static_cast<size_t>(1) << number_of_qubits_, values } {

    check_quantum_state();
}

[[nodiscard]] std::size_t QuantumState::get_number_of_qubits() const {
    return number_of_qubits_;
}

[[nodiscard]] std::size_t QuantumState::get_number_of_bits() const {
    return number_of_bits_;
}

[[nodiscard]] bool QuantumState::is_normalized() {
    return is_null(data_.norm() - 1.);
}

[[nodiscard]] std::vector<std::complex<double>> QuantumState::to_vector() const {
    return data_.to_vector();
}

void QuantumState::reset_data() {
    data_.clear();
    data_[BasisVector{ number_of_qubits_ }] = SparseComplex{ 1. };  // start initialized in state 00...000
}

void QuantumState::reset() { reset_data();
}

[[nodiscard]] double QuantumState::get_probability_of_measuring_one(QubitIndex qubit_index) {
    return data_.accumulate(double{}, [qubit_index](auto total, auto const &kv) {
        auto const &[basis_vector, sparse_complex] = kv;
        if (basis_vector.test(qubit_index.value)) {
            total += std::norm(sparse_complex.value);
        }
        return total;
    });
}

[[nodiscard]] double QuantumState::get_probability_of_measuring_zero(QubitIndex qubit_index) {
    return 1.0 - get_probability_of_measuring_one(qubit_index);
}

// Update data after a measurement
//
// measured_state will be true if we measured a 1, or false if we measured a 0
//
// Example:
//   Given the following data_ representing a quantum state,
//   where data_ is a map of basis vectors to amplitudes,
//   basis vectors are shown below with qubit indices growing from right to left,
//   and amplitudes are complex numbers:
//     Basis vector: amplitude
//            q1 q0: (real, imag)
//             0  0: ( 0.7,    0)
//             0  1: (   0,    0)
//             1  0: (   0,    0)
//             1  1: ( 0.7,    0)
//   And considering qubit 0 was measured as 0.
//   This function:
//     1) Erases all the entries in data_ for which qubit 0 is not 0, i.e., entry 11.
//     2) Normalizes data_ (all the squares of all the amplitudes add up to 1).
void QuantumState::update_data_after_measurement(QubitIndex qubit_index, bool measured_state, double probability_of_measuring_one) {
    data_.erase_if([qubit_index, measured_state](auto const &kv) {
        auto const &[basis_vector, _] = kv;
        auto current_state = basis_vector.test(qubit_index.value);
        return current_state != measured_state;
    });
    data_ *= std::sqrt(1 / (measured_state ? probability_of_measuring_one : (1 - probability_of_measuring_one)));
}

// Update data_ after a reset of a single qubit
//
// Example:
//   Given the following data_ representing a quantum state,
//   where data_ is a map of basis vectors to amplitudes,
//   basis vectors are shown below with qubit indices growing from right to left,
//   and amplitudes are complex numbers:
//     Basis vector: amplitude
//            q1 q0: (real, imag)
//             0  0: ( 0.7,    0)
//             0  1: (   0,    0)
//             1  0: (   0,    0)
//             1  1: ( 0.7,    0)
//   And considering qubit 0 is the one being reset.
//   This function would update data_ as follows:
//     Basis vector: amplitude
//            q1 q0: (real, imag)
//             0  0: ( 0.7,    0)
//             0  1: (   0,    0)  <-- 01 is reset to 00, old 01 amplitude added to 00, 01 amplitude set to 0
//             1  0: ( 0.7,    0)
//             1  1: (   0,    0)  <-- 11 is reset to 10, old 11 amplitude added to 10, 11 amplitude set to 0
void QuantumState::update_data_after_reset(QubitIndex qubit_index) {
    auto newData = data_;
    data_.for_each([qubit_index, &newData](auto const &kv) {
        auto const &[basis_vector, amplitude] = kv;
        if (basis_vector.test(qubit_index.value)) {
            auto basis_vector_after_reset = basis_vector;
            basis_vector_after_reset.set(qubit_index.value, false);
            newData[basis_vector_after_reset].value = std::sqrt(
                std::norm(newData[basis_vector_after_reset].value) + std::norm(amplitude.value)
            );
            newData[basis_vector].value = 0;
        }
    });
    data_ = std::move(newData);
}

// reset does not modify the measurement register
void QuantumState::apply_reset(QubitIndex qubit_index) {
    update_data_after_reset(qubit_index);
}

void QuantumState::apply_reset_all() {
    reset_data();
}

std::ostream& operator<<(std::ostream &os, const QuantumState &state) {
    return os << fmt::format("[{}]", fmt::join(state.to_vector(), ", "));
}

}  // namespace qx::core
