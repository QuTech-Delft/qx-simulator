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
#include "qx/SparseArray.hpp"


namespace qx::core {

struct QuantumStateError : public std::runtime_error {
    explicit QuantumStateError(const std::string &message) : std::runtime_error{ message } {}
};


template <std::size_t NumberOfOperands>
void applyImpl(DenseUnitaryMatrix<1 << NumberOfOperands> const &matrix,
               std::array<QubitIndex, NumberOfOperands> const &operands,
               BasisVector index,
               SparseComplex sparseComplex,
               SparseArray::MapBasisVectorToSparseComplex &storage) {
    utils::Bitset<NumberOfOperands> reducedIndex;
    for (std::size_t i = 0; i < NumberOfOperands; ++i) {
        reducedIndex.set(i, index.test(operands[NumberOfOperands - i - 1].value));
    }
    for (std::size_t i = 0; i < (1 << NumberOfOperands); ++i) {
        std::complex<double> addedValue = sparseComplex.value * matrix.at(i, reducedIndex.toSizeT());
        if (isNotNull(addedValue)) {
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
public:
    explicit QuantumState(std::size_t n);
    [[nodiscard]] std::size_t getNumberOfQubits() const;
    void reset();
    void testInitialize(std::initializer_list<std::pair<std::string, std::complex<double>>> values);

    template <std::size_t NumberOfOperands>
    QuantumState &apply(DenseUnitaryMatrix<1 << NumberOfOperands> const &m,
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

    template <typename F>
    void measure(QubitIndex qubitIndex, F &&randomGenerator) {
        auto rand = randomGenerator();
        double probabilityOfMeasuringOne = 0.;
        data.forEach([qubitIndex, &probabilityOfMeasuringOne](auto const &kv) {
            auto const &[basisVector, sparseComplex] = kv;
            if (basisVector.test(qubitIndex.value)) {
                probabilityOfMeasuringOne += std::norm(sparseComplex.value);
            }
        });
        if (rand < probabilityOfMeasuringOne) {
            data.eraseIf([qubitIndex](auto const &kv) {
                auto const &[basisVector, _] = kv;
                return !basisVector.test(qubitIndex.value);
            });
            data *= std::sqrt(1 / probabilityOfMeasuringOne);
            measurementRegister.set(qubitIndex.value, true);
        } else {
            data.eraseIf([qubitIndex](auto const &kv) {
                auto const &[basisVector, _] = kv;
                return basisVector.test(qubitIndex.value);
            });
            data *= std::sqrt(1 / (1 - probabilityOfMeasuringOne));
            measurementRegister.set(qubitIndex.value, false);
        }
    }

    template <typename F>
    void measureAll(F &&randomGenerator) {
        auto rand = randomGenerator();
        double probability = 0.;
        auto [basisVector, sparseComplex] = std::invoke([this, &probability, rand] {
            for (auto const &[bv, sc] : data) {  // does this work with non-ordered iteration?
                probability += std::norm(sc.value);
                if (probability > rand) {
                    return SparseElement{ bv, sc };
                }
            }
            throw std::runtime_error{ "Vector was not normalized at measurement location (a bug)" };
        });
        data.clear();
        data[basisVector] = SparseComplex{ sparseComplex.value / std::abs(sparseComplex.value) };
        measurementRegister = basisVector;
    }

    template <typename F>
    void prep(QubitIndex qubitIndex, F &&randomGenerator) {
        // Measure + conditional X, and reset the measurement register.
        auto rand = randomGenerator();
        double probabilityOfMeasuringOne = 0.;
        data.forEach([qubitIndex, &probabilityOfMeasuringOne](auto const &kv) {
            auto const &[basisVector, sparseComplex] = kv;
            if (basisVector.test(qubitIndex.value)) {
                probabilityOfMeasuringOne += std::norm(sparseComplex.value);
            }
        });
        if (rand < probabilityOfMeasuringOne) {
            data.eraseIf([qubitIndex](auto const &kv) {
                auto const &[basisVector, _] = kv;
                return !basisVector.test(qubitIndex.value);
            });
            SparseArray::MapBasisVectorToSparseComplex newData;
            for (auto [basisVector, sparseComplex] : data) {
                auto newKey = basisVector;
                newKey.set(qubitIndex.value, false);
                newData.insert(std::make_pair(
                    newKey,
                    SparseComplex{ sparseComplex.value * std::sqrt(1 / probabilityOfMeasuringOne) }
                ));
            }
            data = newData;  // could fix the interface
        } else {
            data.eraseIf([qubitIndex](auto const &kv) {
                auto const &[basisVector, _] = kv;
                return basisVector.test(qubitIndex.value);
            });
            data *= std::sqrt(1 / (1 - probabilityOfMeasuringOne));
        }
        measurementRegister.set(qubitIndex.value, false);
    };

private:
    std::size_t const numberOfQubits = 1;
    SparseArray data;
    BasisVector measurementRegister{};
};

}  // namespace qx::core
