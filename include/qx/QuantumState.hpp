#pragma once

#include <array>
#include <cstdint>  // size_t
#include <complex>  // norm
#include <initializer_list>
#include <string>
#include <utility>  // invoke, pair

#include "qx/Core.hpp"  // BasisVector, QubitIndex
#include "qx/CompileTimeConfiguration.hpp"  // MAX_QUBIT_NUMBER
#include "qx/DenseUnitaryMatrix.hpp"
#include "qx/SparseArray.hpp"


namespace qx::core {

class QuantumState {

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

public:
    explicit QuantumState(std::size_t n);

    [[nodiscard]] std::size_t getNumberOfQubits() const;

    void reset();

    void testInitialize(
        std::initializer_list<std::pair<std::string, std::complex<double>>> values);

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

        data.applyLinear([&m, &operands, this](auto index, auto value, auto &storage) {
            this->applyImpl<NumberOfOperands>(m, operands, index, value, storage); });

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
            if (kv.first.test(qubitIndex.value)) {
                probabilityOfMeasuringOne += std::norm(kv.second);
            }
        });

        if (rand < probabilityOfMeasuringOne) {
            data.eraseIf([qubitIndex](auto const &kv) {
                return !kv.first.test(qubitIndex.value);
            });
            data *= std::sqrt(1 / probabilityOfMeasuringOne);
            measurementRegister.set(qubitIndex.value, true);
        } else {
            data.eraseIf([qubitIndex](auto const &kv) {
                return kv.first.test(qubitIndex.value);
            });
            data *= std::sqrt(1 / (1 - probabilityOfMeasuringOne));
            measurementRegister.set(qubitIndex.value, false);
        }
    }

    template <typename F>
    void measureAll(F &&randomGenerator) {
        auto rand = randomGenerator();
        double probability = 0.;

        auto measuredState = std::invoke([this, &probability, rand] {
            for (auto const &kv :
                 data) { // Does this work with non-ordered iteration?
                probability += std::norm(kv.second);
                if (probability > rand) {
                    return kv;
                }
            }
            throw std::runtime_error(
                "Vector was not normalized at measurement location (a bug)");
        });

        data.clear();
        data.set(measuredState.first,
                 measuredState.second / std::abs(measuredState.second));
        measurementRegister = measuredState.first;
    }

    template <typename F>
    void prep(QubitIndex qubitIndex, F &&randomGenerator) {
        // Measure + conditional X, and reset the measurement register.
        auto rand = randomGenerator();
        double probabilityOfMeasuringOne = 0.;

        data.forEach([qubitIndex, &probabilityOfMeasuringOne](auto const &kv) {
            if (kv.first.test(qubitIndex.value)) {
                probabilityOfMeasuringOne += std::norm(kv.second);
            }
        });

        if (rand < probabilityOfMeasuringOne) {
            data.eraseIf([qubitIndex](auto const &kv) {
                return !kv.first.test(qubitIndex.value);
            });
            SparseArray::Map newData;
            for (auto kv : data.data) {
                auto newKey = kv.first;
                newKey.set(qubitIndex.value, false);
                newData.insert(std::make_pair(
                    newKey,
                    kv.second * std::sqrt(1 / probabilityOfMeasuringOne)));
            }
            data.data = newData; // Could fix the interface
        } else {
            data.eraseIf([qubitIndex](auto const &kv) {
                return kv.first.test(qubitIndex.value);
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
