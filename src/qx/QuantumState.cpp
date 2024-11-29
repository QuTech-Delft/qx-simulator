#include "qx/QuantumState.hpp"

#include <complex>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <ostream>


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
    , data{ static_cast<size_t>(1) << numberOfQubits } {

    resetData();
    checkQuantumState();
}

QuantumState::QuantumState(std::size_t qubit_register_size, std::size_t bit_register_size,
                           std::initializer_list<std::pair<std::string, std::complex<double>>> values)
    : numberOfQubits{ qubit_register_size }
    , numberOfBits{ bit_register_size }
    , data{ static_cast<size_t>(1) << numberOfQubits, values } {

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

[[nodiscard]] std::vector<std::complex<double>> QuantumState::toVector() const {
    return data.toVector();
}

void QuantumState::resetData() {
    data.clear();
    data[BasisVector{}] = SparseComplex{ 1. };  // start initialized in state 00...000
}

void QuantumState::reset() {
    resetData();
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
//
// Example:
//   Given the following data representing a quantum state,
//   where data is a map of basis vectors to amplitudes,
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
//     1) Erases all the entries in data for which qubit 0 is not 0, i.e., entry 11.
//     2) Normalizes data (all the squares of all the amplitudes add up to 1).
void QuantumState::updateDataAfterMeasurement(QubitIndex qubitIndex, bool measuredState, double probabilityOfMeasuringOne) {
    data.eraseIf([qubitIndex, measuredState](auto const &kv) {
        auto const &[basisVector, _] = kv;
        auto currentState = basisVector.test(qubitIndex.value);
        return currentState != measuredState;
    });
    data *= std::sqrt(1 / (measuredState ? probabilityOfMeasuringOne : (1 - probabilityOfMeasuringOne)));
}

// Update data after a reset of a single qubit
//
// Example:
//   Given the following data representing a quantum state,
//   where data is a map of basis vectors to amplitudes,
//   basis vectors are shown below with qubit indices growing from right to left,
//   and amplitudes are complex numbers:
//     Basis vector: amplitude
//            q1 q0: (real, imag)
//             0  0: ( 0.7,    0)
//             0  1: (   0,    0)
//             1  0: (   0,    0)
//             1  1: ( 0.7,    0)
//   And considering qubit 0 is the one being reset.
//   This function would update data as follows:
//     Basis vector: amplitude
//            q1 q0: (real, imag)
//             0  0: ( 0.7,    0)
//             0  1: (   0,    0)  <-- 01 is reset to 00, old 01 amplitude added to 00, 01 amplitude set to 0
//             1  0: ( 0.7,    0)
//             1  1: (   0,    0)  <-- 11 is reset to 10, old 11 amplitude added to 10, 11 amplitude set to 0
void QuantumState::updateDataAfterReset(QubitIndex qubitIndex) {
    auto newData = data;
    data.forEach([qubitIndex, &newData](auto const &kv) {
        auto const &[basisVector, amplitude] = kv;
        if (basisVector.test(qubitIndex.value)) {
            auto basisVectorAfterReset = basisVector;
            basisVectorAfterReset.set(qubitIndex.value, false);
            newData[basisVectorAfterReset].value = std::sqrt(
                std::norm(newData[basisVectorAfterReset].value) + std::norm(amplitude.value)
            );
            newData[basisVector].value = 0;
        }
    });
    data = std::move(newData);
}

std::ostream& operator<<(std::ostream &os, const QuantumState &state) {
    return os << fmt::format("[{}]", fmt::join(state.toVector(), ", "));
}

}  // namespace qx::core
