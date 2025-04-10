#include "qx/sparse_array.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>  // erase_if
#include <complex>
#include <ostream>

namespace qx::core {

SparseArrayError::SparseArrayError(const std::string& message)
: std::runtime_error{ message } {}

SparseComplex::SparseComplex(std::complex<double> c) {
    value = c;
}

bool compare_sparse_elements(const SparseElement& lhs, const SparseElement& rhs) {
    return lhs.first < rhs.first;
}

SparseArray::SparseArray(std::size_t s)
: size_{ s } {}

SparseArray::SparseArray(std::size_t s, std::initializer_list<PairBasisVectorStringComplex> values)
: size_{ s } {
    for (const auto& [basis_vector_string, complex_value] : values) {
        if ((static_cast<size_t>(1) << basis_vector_string.size()) > s) {
            throw SparseArrayError{ fmt::format(
                "found value '{}' for a sparse array of size {}", basis_vector_string, s) };
        }
        data_[BasisVector{ basis_vector_string }] = SparseComplex{ complex_value };
    }
}

SparseArray& SparseArray::operator*=(double d) {
    for (auto& [_, sparse_complex] : data_) {
        sparse_complex.value *= d;
    }
    return *this;
}

SparseComplex& SparseArray::operator[](const BasisVector& index) {
#ifndef NDEBUG
    if (index.to_ulong() >= size_) {
        throw SparseArrayError{ "index out of bounds" };
    }
#endif
    return data_[index];
}

[[nodiscard]] SparseArray::ConstIterator SparseArray::begin() const {
    return data_.cbegin();
}

[[nodiscard]] SparseArray::ConstIterator SparseArray::end() const {
    return data_.cend();
}

[[nodiscard]] SparseArray::Iterator SparseArray::begin() {
    return data_.begin();
}

[[nodiscard]] SparseArray::Iterator SparseArray::end() {
    return data_.end();
}

void SparseArray::clear() {
    data_.clear();
}

[[nodiscard]] std::size_t SparseArray::size() const {
    return size_;
}

[[nodiscard]] double SparseArray::norm() {
    return accumulate<double>(0., [](double total, const auto& kv) {
        const auto& [basis_vector, sparse_complex] = kv;
        return total + std::norm(sparse_complex.value);
    });
}

[[nodiscard]] std::vector<std::complex<double>> SparseArray::to_vector() const {
    auto result = std::vector<std::complex<double>>(size_, 0.);
    for (const auto& [basis_vector, sparse_complex] : data_) {
        result[basis_vector.to_ulong()] = sparse_complex.value;
    }
    return result;
}

void SparseArray::clean_up_zeros() {
    std::erase_if(data_, [](const auto& kv) {
        const auto& [_, sparse_complex] = kv;
        return is_null(sparse_complex.value);
    });
    zero_counter_ = 0;
}

std::ostream& operator<<(std::ostream& os, const SparseArray& array) {
    return os << fmt::format("[{}]", fmt::join(array.to_vector(), ", "));
}

}  // namespace qx::core
