#pragma once

#include <array>
#include <cstdint>  // size_t
#include <complex>  // norm
#include <fmt/ostream.h>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>  // invoke, pair

#include "qx/core.hpp"  // BasisVector, BitMeasurementRegister, MeasurementRegister, QubitIndex
#include "qx/dense_unitary_matrix.hpp"
#include "qx/gates.hpp"
#include "qx/sparse_array.hpp"
#include "qx/utils.hpp"

namespace qx::core {

struct QuantumStateError : public std::runtime_error {
    explicit QuantumStateError(const std::string &message) : std::runtime_error{ message } {}
};


template <std::size_t NumberOfOperands>
void apply_impl(matrix_t<NumberOfOperands> const &matrix,
                operands_t<NumberOfOperands> const &operands,
                BasisVector index,
                SparseComplex const &sparse_complex,
                SparseArray::MapBasisVectorToSparseComplex &storage) {
    BasisVector reduced_index{ NumberOfOperands };
    for (std::size_t i = 0; i < NumberOfOperands; ++i) {
        reduced_index.set(i, index.test(operands[NumberOfOperands - i - 1].value));
    }
    for (std::size_t i = 0; i < (1 << NumberOfOperands); ++i) {
        std::complex<double> added_value = sparse_complex.value * matrix.at(i, reduced_index.to_ulong());
        if (not is_null(added_value)) {
            auto new_index = index;
            for (std::size_t k = 0; k < NumberOfOperands; ++k) {
                new_index.set(operands[NumberOfOperands - k - 1].value, utils::get_bit(i, k));
            }
            auto it = storage.try_emplace(new_index, SparseComplex{ 0. });
            it.first->second.value += added_value;
        }
    }
}


class QuantumState {
    void check_quantum_state();

    void reset_data();

public:
    QuantumState();
    QuantumState(std::size_t qubit_register_size, std::size_t bit_register_size);
    QuantumState(std::size_t qubit_register_size, std::size_t bit_register_size,
                 std::initializer_list<PairBasisVectorStringComplex> values);
    [[nodiscard]] std::size_t get_number_of_qubits() const;
    [[nodiscard]] std::size_t get_number_of_bits() const;
    [[nodiscard]] bool is_normalized();
    [[nodiscard]] std::vector<std::complex<double>> to_vector() const;
    void reset();

    template <std::size_t NumberOfOperands>
    QuantumState &apply(matrix_t<NumberOfOperands> const &m, operands_t<NumberOfOperands> const &operands) {
        assert(NumberOfOperands <= number_of_qubits_ &&
               "Quantum gate has more operands than the number of qubits in this quantum state");
        assert(std::find_if(operands.begin(), operands.end(),
            [this](auto qubit_index) {
                return qubit_index.value >= number_of_qubits_; }
            ) == operands.end()
            && "Operand refers to a non-existing qubit");

        data_.apply_linear([&m, &operands](auto index, auto value, auto &storage) {
            apply_impl<NumberOfOperands>(m, operands, index, value, storage);
        });
        return *this;
    }

    template <typename F>
    void for_each(F &&f) {
        data_.for_each_sorted(f);
    }

    [[nodiscard]] double get_probability_of_measuring_one(QubitIndex qubit_index);
    [[nodiscard]] double get_probability_of_measuring_zero(QubitIndex qubit_index);
    void update_data_after_measurement(QubitIndex qubit_index, bool measured_state, double probability_of_measuring_one);
    void update_data_after_reset(QubitIndex qubit_index);

    // measured_state will be true if we measured a 1, or false if we measured a 0
    template <typename F>
    void apply_measure(QubitIndex qubit_index, BitIndex bit_index, F &&random_generator,
                       core::MeasurementRegister &measurement_register,
                       core::BitMeasurementRegister &bit_measurement_register) {
        auto probability_of_measuring_one = get_probability_of_measuring_one(qubit_index);
        auto measured_state = (random_generator() < probability_of_measuring_one);
        update_data_after_measurement(qubit_index, measured_state, probability_of_measuring_one);
        measurement_register.set(qubit_index.value, measured_state);
        bit_measurement_register.set(bit_index.value, measured_state);
    }

    void apply_reset(QubitIndex qubit_index);
    void apply_reset_all();

private:
    std::size_t number_of_qubits_;
    std::size_t number_of_bits_;
    SparseArray data_;
};

std::ostream& operator<<(std::ostream &os, const QuantumState &array);

}  // namespace qx::core

template <> struct fmt::formatter<qx::core::QuantumState> : fmt::ostream_formatter {};
