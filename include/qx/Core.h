#pragma once

#include "absl/container/flat_hash_map.h"
#include <cassert>
#include <complex>
#include <limits>

#include "qx/Common.h"
#include "qx/CompileTimeConfiguration.h"

namespace qx {
namespace core {

inline constexpr bool isNotNull(std::complex<double> c) {
#if defined(_MSC_VER)
    return c.real() > config::EPS || -c.real() > config::EPS ||
           c.imag() > config::EPS || -c.imag() > config::EPS;
#else
    return std::abs(c.real()) > config::EPS || std::abs(c.imag()) > config::EPS;
#endif
}

struct QubitIndex {
    std::size_t value;
};

template <std::size_t N> class DenseUnitaryMatrix {
public:
    using Matrix = std::array<std::array<std::complex<double>, N>, N>;

    static constexpr DenseUnitaryMatrix<N> identity() {
        Matrix m;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = (i == j) ? 1 : 0;
            }
        }

        return DenseUnitaryMatrix(m, false);
    }

    explicit constexpr DenseUnitaryMatrix(Matrix const &m)
        : DenseUnitaryMatrix(m, true) {}

    inline constexpr std::complex<double> at(std::size_t i,
                                             std::size_t j) const {
        return matrix[i][j];
    }

    constexpr DenseUnitaryMatrix<N> dagger() const {
        Matrix m;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = std::conj(at(j, i));
            }
        }

        return DenseUnitaryMatrix(m, false);
    }

    constexpr bool operator==(DenseUnitaryMatrix<N> const &other) const {
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                if (isNotNull(at(i, j) - other.at(i, j))) {
                    return false;
                }
            }
        }
        return true;
    }

    constexpr DenseUnitaryMatrix<N>
    operator*(DenseUnitaryMatrix<N> const &other) const {
        Matrix m;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = 0;
                for (std::size_t k = 0; k < N; ++k) {
                    m[i][j] += at(i, k) * other.at(k, j);
                }
            }
        }

        return DenseUnitaryMatrix(m, false);
    }

private:
    constexpr DenseUnitaryMatrix(Matrix const &m, bool checkIsUnitary)
        : matrix(m) {
        if (checkIsUnitary) {
            checkUnitary();
        }
    }

    constexpr void checkUnitary() const {
        if (!(*this * dagger() == identity())) {
            throw std::runtime_error("Matrix is not unitary");
        };
    }

    std::array<std::array<std::complex<double>, N>, N> const matrix;
};

class QuantumState;

class SparseArray {
public:
    using Map = absl::flat_hash_map<BasisVector, std::complex<double>>;
    using Iterator = Map::const_iterator;

    SparseArray() = delete;

    explicit SparseArray(std::size_t s) : size(s){};

    std::size_t getSize() const { return size; }

    std::vector<std::complex<double>> testToVector() const {
        std::vector<std::complex<double>> result(getSize(), 0);

        for (auto const &kv : *this) {
            result[kv.first.toSizeT()] = kv.second;
        }

        return result;
    }

    Iterator begin() const { return data.cbegin(); }

    Iterator end() const { return data.cend(); }

    void set(BasisVector index, std::complex<double> value);

    void clear() { data.clear(); }

    SparseArray &operator*=(double d) {
        std::for_each(data.begin(), data.end(),
                      [d](auto &kv) { kv.second *= d; });
        return *this;
    }

    template <typename F> void forEach(F &&f) {
        cleanupZeros();
        std::for_each(data.begin(), data.end(), f);
    }

    template <typename F> void forEachSorted(F &&f) {
        cleanupZeros();
        std::vector<std::pair<BasisVector, std::complex<double>>> sorted(
            data.begin(), data.end());
        std::sort(sorted.begin(), sorted.end(),
                  [](auto const &left, auto const &right) {
                      return left.first < right.first;
                  });
        std::for_each(sorted.begin(), sorted.end(), f);
    }

    template <typename F> void eraseIf(F &&pred) { absl::erase_if(data, pred); }

private:
    friend QuantumState;

    // Let f build a new SparseArray to replace *this, assuming f is linear.
    template <typename F> void applyLinear(F &&f) {
        // Every ZERO_CYCLE_SIZE gates, cleanup the 0s
        if (zeroCounter >= config::ZERO_CYCLE_SIZE) {
            cleanupZeros();
        }
        ++zeroCounter;

        Map result;

        for (auto const &kv : data) {
            f(kv.first, kv.second, result);
        }

        data.swap(result);
    }

    void cleanupZeros();

    std::size_t const size = 0;
    std::uint64_t zeroCounter = 0;
    Map data;
};

class QuantumState {
public:
    explicit QuantumState(std::size_t n)
        : numberOfQubits(n), data(1 << numberOfQubits) {
        assert(numberOfQubits > 0 && "QuantumState needs at least one qubit");
        assert(numberOfQubits <= config::MAX_QUBIT_NUMBER &&
               "QuantumState currently cannot support that many qubits with "
               "this version of OpenQL");
        data.set(BasisVector{}, 1); // Start initialized in state 00...000
    };

    std::size_t getNumberOfQubits() const { return numberOfQubits; }

    void reset() {
        data.clear();
        data.set(BasisVector{}, 1); // Start initialized in state 00...000
    }

    void testInitialize(
        std::initializer_list<std::pair<std::string, std::complex<double>>>
            values);

    template <std::size_t NumberOfOperands>
    QuantumState &
    apply(DenseUnitaryMatrix<1 << NumberOfOperands> const &m,
          std::array<QubitIndex, NumberOfOperands> const &operands);

    template <typename F> void forEach(F &&f) { data.forEachSorted(f); }

    BasisVector getMeasurementRegister() const { return measurementRegister; }

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

    template <typename F> void measureAll(F &&randomGenerator) {
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

} // namespace core
} // namespace qx