#include <fmt/format.h>

#include "qx/QuantumState.hpp"


namespace qx::core {

QuantumState::QuantumState(std::size_t n)
    : numberOfQubits(n),
      data(1 << numberOfQubits) {
    if (numberOfQubits == 0) {
        throw QuantumStateError{ "quantum state needs at least one qubit" };
    }
    if (numberOfQubits > config::MAX_QUBIT_NUMBER) {
        throw QuantumStateError{ fmt::format("quantum state size exceeds maximum allowed: {} > {}", numberOfQubits,
                                             config::MAX_QUBIT_NUMBER) };
    }
    data.set(BasisVector{}, std::complex<double>{ 1 });  // Start initialized in state 00...000
};

[[nodiscard]] std::size_t QuantumState::getNumberOfQubits() const {
    return numberOfQubits;
}

void QuantumState::reset() {
    data.clear();
    data.set(BasisVector{}, std::complex<double>{ 1 });  // Start initialized in state 00...000
    measurementRegister.reset();
}

void QuantumState::testInitialize(
    std::initializer_list<std::pair<std::string, std::complex<double>>> values) {
    data.clear();
    double norm{};
    for (auto const &[state_string, amplitude] : values) {
        data.set(BasisVector{ state_string }, amplitude);
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

[[nodiscard]] const BasisVector &QuantumState::getMeasurementRegister() const {
    return measurementRegister;
}

}  // namespace qx::core
