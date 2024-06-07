#include "qx/QuantumState.hpp"


namespace qx::core {

QuantumState::QuantumState(std::size_t n)
    : numberOfQubits(n), data(1 << numberOfQubits) {
    assert(numberOfQubits > 0 && "QuantumState needs at least one qubit");
    assert(numberOfQubits <= config::MAX_QUBIT_NUMBER &&
           "QuantumState currently cannot support that many qubits with this version of QX-simulator");
    data.set(BasisVector{}, 1);  // Start initialized in state 00...000
};

[[nodiscard]] std::size_t QuantumState::getNumberOfQubits() const {
    return numberOfQubits;
}

void QuantumState::reset() {
    data.clear();
    data.set(BasisVector{}, 1);  // Start initialized in state 00...000
    measurementRegister.reset();
}

void QuantumState::testInitialize(
    std::initializer_list<std::pair<std::string, std::complex<double>>> values) {
    data.clear();
    double norm = 0;
    for (auto const &kv: values) {
        BasisVector index(kv.first);
        data.set(index, kv.second);
        norm += std::norm(kv.second);
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
