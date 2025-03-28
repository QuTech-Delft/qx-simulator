#pragma once

#include <fmt/ostream.h>

#include <algorithm>  // erase_if, for_each, sort
#include <complex>
#include <cstdint>  // size_t, uint64_t
#include <numeric>  // accumulate
#include <ostream>
#include <stdexcept>  // runtime_error
#include <unordered_map>
#include <utility>  // pair
#include <vector>

#include "qx/compile_time_configuration.hpp"  // ZERO_CYCLE_SIZE
#include "qx/core.hpp"  // BasisVector, Complex, PairBasisVectorStringComplex, QubitIndex

namespace qx::core {

class QuantumState;

struct SparseArrayError : public std::runtime_error {
    explicit SparseArrayError(const std::string& message);
};

struct SparseComplex {
    std::complex<double> value;

    SparseComplex() = default;
    explicit SparseComplex(std::complex<double> c);
    SparseComplex(const SparseComplex& other);
    SparseComplex(SparseComplex&& other) noexcept;
    SparseComplex& operator=(const SparseComplex& other);
    SparseComplex& operator=(SparseComplex&& other) noexcept;
};
using SparseElement = std::pair<BasisVector, SparseComplex>;
bool compare_sparse_elements(const SparseElement& lhs, const SparseElement& rhs);

class SparseArray {
public:
    using MapBasisVectorToSparseComplex = std::unordered_map<BasisVector, SparseComplex>;
    using VectorOfSparseElements = std::vector<SparseElement>;
    using Iterator = MapBasisVectorToSparseComplex::iterator;
    using ConstIterator = MapBasisVectorToSparseComplex::const_iterator;

public:
    SparseArray() = delete;
    explicit SparseArray(std::size_t s);
    SparseArray(std::size_t s, std::initializer_list<PairBasisVectorStringComplex> values);
    SparseArray(const SparseArray& other) = default;
    SparseArray(SparseArray&& other) noexcept = default;
    SparseArray& operator=(const SparseArray& other);
    SparseArray& operator=(SparseArray&& other) noexcept = default;
    ~SparseArray() = default;

    [[nodiscard]] ConstIterator begin() const;
    [[nodiscard]] ConstIterator end() const;
    [[nodiscard]] Iterator begin();
    [[nodiscard]] Iterator end();

    SparseArray& operator*=(double d);
    SparseComplex& operator[](const BasisVector& index);

    void clear();
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] double norm();
    [[nodiscard]] std::vector<std::complex<double>> to_vector() const;

    template <typename T, typename F>
    T accumulate(T init, F&& f) {
        clean_up_zeros();
        return std::accumulate(data_.begin(), data_.end(), init, f);
    }

    template <typename F>
    void for_each(F&& f) {
        clean_up_zeros();
        std::for_each(data_.begin(), data_.end(), f);
    }

    template <typename F>
    void for_each_sorted(F&& f) {
        clean_up_zeros();
        VectorOfSparseElements sorted(data_.begin(), data_.end());
        std::sort(sorted.begin(), sorted.end(), compare_sparse_elements);
        std::for_each(sorted.begin(), sorted.end(), f);
    }

    template <typename F>
    void erase_if(F&& pred) {
        std::erase_if(data_, pred);
    }

    // Let f build a new SparseArray to replace *this, assuming f is linear.
    template <typename F>
    void apply_linear(F&& f) {
        // Every ZERO_CYCLE_SIZE gates, cleanup the 0s
        if (zero_counter_ >= config::ZERO_CYCLE_SIZE) {
            clean_up_zeros();
        }
        ++zero_counter_;
        MapBasisVectorToSparseComplex result;
        for (const auto& [basis_vector, complex_value] : data_) {
            f(basis_vector, complex_value, result);
        }
        data_.swap(result);
    }

private:
    void clean_up_zeros();

    std::size_t size_ = 0;
    std::uint64_t zero_counter_ = 0;
    MapBasisVectorToSparseComplex data_;
};

std::ostream& operator<<(std::ostream& os, const SparseArray& array);

}  // namespace qx::core

template <>
struct fmt::formatter<std::complex<double>> : fmt::ostream_formatter {};
template <>
struct fmt::formatter<qx::core::SparseArray> : fmt::ostream_formatter {};
