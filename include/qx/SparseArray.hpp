#pragma once

#include <absl/container/flat_hash_map.h>
#include <algorithm>  // for_each, sort
#include <complex>
#include <cstdint>  // size_t, uint64_t
#include <utility>  // pair
#include <vector>

#include "qx/Core.hpp"  // BasisVector, QubitIndex
#include "qx/CompileTimeConfiguration.hpp"  // ZERO_CYCLE_SIZE


namespace qx::core {

class QuantumState;

class SparseArray {
public:
    using Map = absl::flat_hash_map<BasisVector, std::complex<double>>;
    using Iterator = Map::const_iterator;

    SparseArray() = delete;

    explicit SparseArray(std::size_t s);

    [[nodiscard]] std::size_t getSize() const;

    [[nodiscard]] std::vector<std::complex<double>> testToVector() const;

    [[nodiscard]] Iterator begin() const;

    [[nodiscard]] Iterator end() const;

    void set(BasisVector index, std::complex<double> value);

    void clear();

    SparseArray &operator*=(double d);

    template <typename F>
    void forEach(F &&f) {
        cleanupZeros();
        std::for_each(data.begin(), data.end(), f);
    }

    template <typename F>
    void forEachSorted(F &&f) {
        cleanupZeros();
        std::vector<std::pair<BasisVector, std::complex<double>>> sorted(
            data.begin(), data.end());
        std::sort(sorted.begin(), sorted.end(),
                  [](auto const &left, auto const &right) {
                      return left.first < right.first;
                  });
        std::for_each(sorted.begin(), sorted.end(), f);
    }

    template <typename F>
    void eraseIf(F &&pred) {
        absl::erase_if(data, pred);
    }

private:
    friend QuantumState;

    // Let f build a new SparseArray to replace *this, assuming f is linear.
    template <typename F>
    void applyLinear(F &&f) {
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

}  // namespace qx::core
