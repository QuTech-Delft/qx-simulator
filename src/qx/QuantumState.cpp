#include <fmt/format.h>

#include "qx/QuantumState.hpp"


namespace qx::core {

void QuantumState::checkQuantumState() {
    if (numberOfQubits == 0) {
        throw QuantumStateError{ "number of qubits needs to be at least 1" };
    }
    if (numberOfQubits > config::MAX_QUBIT_NUMBER) {
        throw QuantumStateError{ fmt::format("number of qubits exceeds maximum allowed: {} > {}", numberOfQubits,
                                             config::MAX_QUBIT_NUMBER) };
    }
    if (numberOfBits > config::MAX_BIT_NUMBER) {
        throw QuantumStateError{ fmt::format("number of bits exceeds maximum allowed: {} > {}", numberOfBits,
                                             config::MAX_BIT_NUMBER) };
    }
    if (not isNormalized()) {
        throw QuantumStateError{ "quantum state is not normalized" };
    }
}

QuantumState::QuantumState(std::size_t qubit_register_size, std::size_t bit_register_size)
    : numberOfQubits{ qubit_register_size }
    , numberOfBits{ bit_register_size }
    , data{ static_cast<size_t>(1) << numberOfQubits }
    , bitMeasurementRegister{ numberOfBits } {

    data[BasisVector{}] = SparseComplex{ 1. };  // start initialized in state 00...000
    checkQuantumState();
}

QuantumState::QuantumState(std::size_t qubit_register_size, std::size_t bit_register_size,
                           std::initializer_list<std::pair<std::string, std::complex<double>>> values)
    : numberOfQubits{ qubit_register_size }
    , numberOfBits{ bit_register_size }
    , data{ static_cast<size_t>(1) << numberOfQubits, values }
    , bitMeasurementRegister{ numberOfBits } {

    checkQuantumState();
}

[[nodiscard]] std::size_t QuantumState::getNumberOfQubits() const {
    return numberOfQubits;
}

[[nodiscard]] std::size_t QuantumState::getNumberOfBits() const {
    return numberOfBits;
}

[[nodiscard]] bool QuantumState::isNormalized() {
    return isNull(data.norm() - 1.);
}

void QuantumState::reset() {
    data.clear();
    data[BasisVector{}] = SparseComplex{ 1. };  // start initialized in state 00...000
    measurementRegister.reset();
    bitMeasurementRegister.reset();
}

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
