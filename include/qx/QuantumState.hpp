#pragma once

#include <array>
#include <cstdint>  // size_t
#include <complex>  // norm
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <utility>  // invoke, pair

#include "qx/Core.hpp"  // BasisVector, QubitIndex
#include "qx/CompileTimeConfiguration.hpp"  // MAX_QUBIT_NUMBER
#include "qx/DenseUnitaryMatrix.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SparseArray.hpp"


namespace qx::core {

struct QuantumStateError : public std::runtime_error {
    explicit QuantumStateError(const std::string &message) : std::runtime_error{ message } {}
};


template <std::size_t NumberOfOperands>
void applyImpl(DenseUnitaryMatrix<1 << NumberOfOperands> const &matrix,
               std::array<QubitIndex, NumberOfOperands> const &operands,
               BasisVector index,
               SparseComplex const &sparseComplex,
               SparseArray::MapBasisVectorToSparseComplex &storage) {
    utils::Bitset<NumberOfOperands> reducedIndex;
    for (std::size_t i = 0; i < NumberOfOperands; ++i) {
        reducedIndex.set(i, index.test(operands[NumberOfOperands - i - 1].value));
    }
    for (std::size_t i = 0; i < (1 << NumberOfOperands); ++i) {
        std::complex<double> addedValue = sparseComplex.value * matrix.at(i, reducedIndex.toSizeT());
        if (not isNull(addedValue)) {
            auto newIndex = index;
            for (std::size_t k = 0; k < NumberOfOperands; ++k) {
                newIndex.set(operands[NumberOfOperands - k - 1].value, utils::getBit(i, k));
            }
            auto it = storage.try_emplace(newIndex, SparseComplex{ 0. });
            it.first->second.value += addedValue;
        }
    }
}


class QuantumState {
    void checkQuantumState();

public:
    QuantumState(std::size_t qubit_register_size);
    QuantumState(std::size_t qubit_register_size,
                 std::initializer_list<std::pair<std::string, std::complex<double>>> values);
    [[nodiscard]] std::size_t getNumberOfQubits() const;
    [[nodiscard]] bool isNormalized();
    void reset();

    template <std::size_t NumberOfOperands>
    QuantumState &apply(
        DenseUnitaryMatrix<1 << NumberOfOperands> const &m,
        std::array<QubitIndex, NumberOfOperands> const &operands) {

        assert(NumberOfOperands <= numberOfQubits &&
               "Quantum gate has more operands than the number of qubits in this quantum state");
        assert(std::find_if(operands.begin(), operands.end(),
                            [this](auto qubitIndex) {
                                return qubitIndex.value >= numberOfQubits;
                            }) == operands.end() &&
               "Operand refers to a non-existing qubit");

        data.applyLinear([&m, &operands](auto index, auto value, auto &storage) {
            applyImpl<NumberOfOperands>(m, operands, index, value, storage);
        });
        return *this;
    }

    template <typename F>
    void forEach(F &&f) {
        data.forEachSorted(f);
    }

    [[nodiscard]] const BasisVector &getMeasurementRegister() const;
    [[nodiscard]] double getProbabilityOfMeasuringOne(QubitIndex qubitIndex);
    [[nodiscard]] double getProbabilityOfMeasuringZero(QubitIndex qubitIndex);
    void collapseQubit(QubitIndex qubitIndex, bool measuredState, double measuredStateProbability);

    // measuredState will be true if we measured a 1, or false if we measured a 0
    // measuredStateProbability will be the probability of measuring 1 if we measured a 1,
    //   or the probability of measuring 0 if we measured a 0
    template <typename F>
    void measure(QubitIndex qubitIndex, F &&randomGenerator) {
        auto probabilityOfMeasuringOne = getProbabilityOfMeasuringOne(qubitIndex);
        auto measuredState = (randomGenerator() < probabilityOfMeasuringOne);
        auto measuredStateProbability = measuredState ? probabilityOfMeasuringOne : (1 - probabilityOfMeasuringOne);
        collapseQubit(qubitIndex, measuredState, measuredStateProbability);
        measurementRegister.set(qubitIndex.value, measuredState);
    }

private:
    std::size_t numberOfQubits = 1;
    SparseArray data;
    BasisVector measurementRegister{};
};

}  // namespace qx::core
