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
    if (not isNormalized()) {
        throw QuantumStateError{ "quantum state is not normalized" };
    }
}

QuantumState::QuantumState(std::size_t qubit_register_size)
    : numberOfQubits{ qubit_register_size }
    , data{ static_cast<size_t>(1) << numberOfQubits } {

    data[BasisVector{}] = SparseComplex{ 1. };  // start initialized in state 00...000
    checkQuantumState();
}

QuantumState::QuantumState(std::size_t qubit_register_size,
                           std::initializer_list<std::pair<std::string, std::complex<double>>> values)
    : numberOfQubits{ qubit_register_size }
    , data{ static_cast<size_t>(1) << numberOfQubits, values } {

    checkQuantumState();
}

[[nodiscard]] std::size_t QuantumState::getNumberOfQubits() const {
    return numberOfQubits;
}

[[nodiscard]] bool QuantumState::isNormalized() {
    return isNull(data.norm() - 1.);
}

void QuantumState::reset() {
    data.clear();
    data[BasisVector{}] = SparseComplex{ 1. };  // start initialized in state 00...000
    measurementRegister.reset();
}

[[nodiscard]] const BasisVector& QuantumState::getMeasurementRegister() const {
    return measurementRegister;
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

// Update data after a measurement
//
// measuredState will be true if we measured a 1, or false if we measured a 0
// measuredStateProbability will be the probability of measuring 1 if we measured a 1,
//   or the probability of measuring 0 if we measured a 0
void QuantumState::collapseQubit(QubitIndex qubitIndex, bool measuredState, double measuredStateProbability) {
    data.eraseIf([qubitIndex, measuredState](auto const &kv) {
        auto const &[basisVector, _] = kv;
        auto currentState = basisVector.test(qubitIndex.value);
        return currentState != measuredState;
    });
    data *= std::sqrt(1 / measuredStateProbability);
}

}  // namespace qx::core
