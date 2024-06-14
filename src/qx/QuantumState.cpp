#include <fmt/format.h>

#include "qx/QuantumState.hpp"


namespace qx::core {

QuantumState::QuantumState(std::size_t qubit_register_size, std::size_t bit_register_size)
    : numberOfQubits(qubit_register_size)
    , numberOfBits(bit_register_size)
    , data(static_cast<size_t>(1) << numberOfQubits)
    , bitMeasurementRegister{ numberOfBits } {
    if (numberOfQubits == 0) {
        throw QuantumStateError{ "quantum state needs at least one qubit" };
    }
    if (numberOfQubits > config::MAX_QUBIT_NUMBER) {
        throw QuantumStateError{ fmt::format("quantum state size exceeds maximum allowed: {} > {}", numberOfQubits,
                                             config::MAX_QUBIT_NUMBER) };
    }
    if (numberOfQubits > config::MAX_BIT_NUMBER) {
        throw QuantumStateError{ fmt::format("quantum state size exceeds maximum allowed: {} > {}", numberOfQubits,
                                             config::MAX_QUBIT_NUMBER) };
    }
    data[BasisVector{}] = SparseComplex{ 1. };  // start initialized in state 00...000
};

[[nodiscard]] std::size_t QuantumState::getNumberOfQubits() const {
    return numberOfQubits;
}

[[nodiscard]] std::size_t QuantumState::getNumberOfBits() const {
    return numberOfBits;
}

void QuantumState::reset() {
    data.clear();
    data[BasisVector{}] = SparseComplex{ 1. };  // start initialized in state 00...000
    measurementRegister.reset();
}

void QuantumState::testInitialize(
    std::initializer_list<std::pair<std::string, std::complex<double>>> values) {
    data.clear();
    double norm{};
    for (auto const &[state_string, amplitude] : values) {
        data[BasisVector{ state_string }] = SparseComplex{ amplitude };
        norm += std::norm(amplitude);
    }
    assert(!isNotNull(norm - 1));
}

// Explicit instantiation for use in Circuit::execute, otherwise linking error.

template QuantumState &QuantumState::apply<1>(
    DenseUnitaryMatrix<1 << 1> const &m, std::array<QubitIndex, 1> const &operands);

template QuantumState &QuantumState::apply<2>(
    DenseUnitaryMatrix<1 << 2> const &m, std::array<QubitIndex, 2> const &operands);

template QuantumState &QuantumState::apply<3>(
    DenseUnitaryMatrix<1 << 3> const &m, std::array<QubitIndex, 3> const &operands);

[[nodiscard]] const BasisVector& QuantumState::getMeasurementRegister() const {
    return measurementRegister;
}
[[nodiscard]] const BitMeasurementRegister& QuantumState::getBitMeasurementRegister() const {
    return bitMeasurementRegister;
}

[[nodiscard]] double QuantumState::getProbabilityOfMeasuringOne(QubitIndex qubitIndex) {
    return data.accumulate(double{}, [qubitIndex](auto total, auto const &kv) {
        auto const &[basisVector, sparseComplex] = kv;
        if (basisVector.test(qubitIndex.value)) {
            total += std::norm(sparseComplex.value);
        }
        return total;
    });
}

[[nodiscard]] double QuantumState::getProbabilityOfMeasuringZero(QubitIndex qubitIndex) {
    return 1.0 - getProbabilityOfMeasuringOne(qubitIndex);
}

void QuantumState::collapseQubit(QubitIndex qubitIndex, bool measuredState, double probabilityOfMeasuringOne) {
    data.eraseIf([qubitIndex, measuredState](auto const &kv) {
        auto const &[basisVector, _] = kv;
        auto currentState = basisVector.test(qubitIndex.value);
        return currentState != measuredState;
    });

    auto probability = measuredState
                           ? probabilityOfMeasuringOne
                           : (1 - probabilityOfMeasuringOne);
    data *= std::sqrt(1 / probability);

}

}  // namespace qx::core
