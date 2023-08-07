#include "qx/Core.hpp"

namespace qx::core {

namespace {

template <std::size_t NumberOfOperands>
void applyImpl(DenseUnitaryMatrix<1 << NumberOfOperands> const &matrix,
               std::array<QubitIndex, NumberOfOperands> const &operands,
               BasisVector index, std::complex<double> value,
               SparseArray::Map &storage) {
    utils::Bitset<NumberOfOperands> reducedIndex;
    for (std::size_t i = 0; i < NumberOfOperands; ++i) {
        reducedIndex.set(i, index.test(operands[NumberOfOperands - i - 1].value));
    }

    for (std::size_t i = 0; i < (1 << NumberOfOperands); ++i) {
        std::complex<double> addedValue = value * matrix.at(i, reducedIndex.toSizeT());

        if (isNotNull(addedValue)) {
            auto newIndex = index;

            for (std::size_t k = 0; k < NumberOfOperands; ++k) {
                newIndex.set(operands[NumberOfOperands - k - 1].value, utils::getBit(i, k));
            }

            auto it = storage.try_emplace(newIndex, 0);
            auto newValue = it.first->second + addedValue;
            it.first->second = newValue;
        }
    }
}

} // namespace

void SparseArray::set(BasisVector index, std::complex<double> value) {
#ifndef NDEBUG
    if (index.toSizeT() >= size) {
        throw std::runtime_error("SparseArray::set index out of bounds");
    }
#endif

    if (std::abs(value) < config::EPS) {
        return;
    }

    data.try_emplace(index, value);
}

void SparseArray::cleanupZeros() {
    absl::erase_if(data, [](auto const &kv) { return !isNotNull(kv.second); });
    zeroCounter = 0;
}

void QuantumState::testInitialize(
    std::initializer_list<std::pair<std::string, std::complex<double>>> values) {
    data.clear();
    double norm = 0;
    for (auto const &kv : values) {
        BasisVector index(kv.first);
        data.set(index, kv.second);
        norm += std::norm(kv.second);
    }
    assert(!isNotNull(norm - 1));
}

template <std::size_t NumberOfOperands>
QuantumState &
QuantumState::apply(DenseUnitaryMatrix<1 << NumberOfOperands> const &m,
                    std::array<QubitIndex, NumberOfOperands> const &operands) {
    assert(NumberOfOperands <= numberOfQubits &&
           "Quantum gate has more operands than the number of qubits in this "
           "quantum state");
    assert(std::find_if(operands.begin(), operands.end(),
                        [this](auto qubitIndex) {
                            return qubitIndex.value >= numberOfQubits;
                        }) == operands.end() &&
           "Operand refers to a non-existing qubit");

    data.applyLinear([&m, &operands](auto index, auto value, auto &storage) {
        applyImpl<NumberOfOperands>(m, operands, index, value, storage); });

    return *this;
}

// Explicit instantiation for use in Circuit::execute, otherwise linking error.

template QuantumState &
QuantumState::apply<1>(DenseUnitaryMatrix<1 << 1> const &m,
                       std::array<QubitIndex, 1> const &operands);

template QuantumState &
QuantumState::apply<2>(DenseUnitaryMatrix<1 << 2> const &m,
                       std::array<QubitIndex, 2> const &operands);

template QuantumState &
QuantumState::apply<3>(DenseUnitaryMatrix<1 << 3> const &m,
                       std::array<QubitIndex, 3> const &operands);

} // namespace qx::core
