#pragma once

#include <absl/container/flat_hash_map.h>
#include <algorithm>  // for_each, sort
#include <complex>
#include <cstdint>  // size_t, uint64_t
#include <numeric>  // accumulate
#include <stdexcept>  // runtime_error
#include <utility>  // pair
#include <vector>

#include "qx/Core.hpp"  // BasisVector, Complex, QubitIndex
#include "qx/CompileTimeConfiguration.hpp"  // ZERO_CYCLE_SIZE


namespace qx::core {

class QuantumState;


struct SparseArrayError : public std::runtime_error {
    explicit SparseArrayError(const std::string &message);
};


struct SparseComplex {
    std::complex<double> value;

    SparseComplex() = default;
    explicit SparseComplex(std::complex<double> c);
    SparseComplex(const SparseComplex &other);
    SparseComplex(SparseComplex &&other) noexcept;
    SparseComplex& operator=(const SparseComplex &other);
    SparseComplex& operator=(SparseComplex &&other) noexcept;
};
using SparseElement = std::pair<BasisVector, SparseComplex>;
bool compareSparseElements(const SparseElement &lhs, const SparseElement &rhs);


class SparseArray {
public:
    using MapBasisVectorToSparseComplex = absl::flat_hash_map<BasisVector, SparseComplex>;
    using VectorOfSparseElements = std::vector<SparseElement>;
    using Iterator = MapBasisVectorToSparseComplex::iterator;
    using ConstIterator = MapBasisVectorToSparseComplex::const_iterator;

public:
    SparseArray() = delete;
    explicit SparseArray(std::size_t s);
    SparseArray(std::size_t s, std::initializer_list<std::pair<std::string, std::complex<double>>> values);

    [[nodiscard]] ConstIterator begin() const;
    [[nodiscard]] ConstIterator end() const;
    [[nodiscard]] Iterator begin();
    [[nodiscard]] Iterator end();

    SparseArray& operator=(MapBasisVectorToSparseComplex map);
    SparseArray& operator*=(double d);
    SparseComplex& operator[](const BasisVector &index);

    void clear();
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] double norm();
    [[nodiscard]] std::vector<std::complex<double>> toVector() const;

    template <typename T, typename F>
    T accumulate(T init, F &&f) {
        cleanUpZeros();
        return std::accumulate(data_.begin(), data_.end(), init, f);
    }

    template <typename F>
    void forEach(F &&f) {
        cleanUpZeros();
        std::for_each(data_.begin(), data_.end(), f);
    }

    template <typename F>
    void forEachSorted(F &&f) {
        cleanUpZeros();
        VectorOfSparseElements sorted(data_.begin(), data_.end());
        std::sort(sorted.begin(), sorted.end(), compareSparseElements);
        std::for_each(sorted.begin(), sorted.end(), f);
    }

    template <typename F>
    void eraseIf(F &&pred) {
        absl::erase_if(data_, pred);
    }

    // Let f build a new SparseArray to replace *this, assuming f is linear.
    template <typename F>
    void applyLinear(F &&f) {
        // Every ZERO_CYCLE_SIZE gates, cleanup the 0s
        if (zeroCounter_ >= config::ZERO_CYCLE_SIZE) {
            cleanUpZeros();
        }
        ++zeroCounter_;
        MapBasisVectorToSparseComplex result;
        for (auto const &[basisVector, complex_value] : data_) {
            f(basisVector, complex_value, result);
        }
        data_.swap(result);
    }

private:
    void cleanUpZeros();

    std::size_t size_ = 0;
    std::uint64_t zeroCounter_ = 0;
    MapBasisVectorToSparseComplex data_;
};

}  // namespace qx::core
