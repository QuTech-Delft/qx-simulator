#include "qx/SparseArray.hpp"

#include <complex>
#include <fmt/format.h>


namespace qx::core {

SparseArrayError::SparseArrayError(const std::string &message)
    : std::runtime_error{ message }
{}

SparseComplex::SparseComplex(std::complex<double> c) {
    value = c;
}

SparseComplex::SparseComplex(const SparseComplex &other) {
    value = other.value;
}

SparseComplex::SparseComplex(SparseComplex &&other) noexcept {
    value = std::move(other.value);
}

SparseComplex& SparseComplex::operator=(const SparseComplex &other) {
    if (std::abs(other.value) >= config::EPS) {
        value = other.value;
    }
    return *this;
}

SparseComplex& SparseComplex::operator=(SparseComplex &&other) noexcept {
    if (std::abs(other.value) >= config::EPS) {
        value = std::move(other.value);
    }
    return *this;
}

bool compareSparseElements(const SparseElement &lhs, const SparseElement &rhs) {
    return lhs.first < rhs.first;
}

SparseArray::SparseArray(std::size_t s)
    : size_{ s }
{}

SparseArray::SparseArray(std::size_t s, std::initializer_list<std::pair<std::string, std::complex<double>>> values)
    : size_{ s } {
    for (auto const &[basis_vector_string, complex_value] : values) {
        if ((static_cast<size_t>(1) << basis_vector_string.size()) > s) {
            throw SparseArrayError{
                fmt::format("found value '{}' for a sparse array of size {}", basis_vector_string, s) };
        }
        data_[BasisVector{ basis_vector_string }] = SparseComplex{ complex_value };
    }
}

SparseArray& SparseArray::operator=(MapBasisVectorToSparseComplex map) {
    data_ = std::move(map);
    return *this;
}

SparseArray& SparseArray::operator*=(double d) {
    for (auto &[_, sparseComplex] : data_) {
        sparseComplex.value *= d;
    }
    return *this;
}

SparseComplex& SparseArray::operator[](const BasisVector &index) {
#ifndef NDEBUG
    if (index.toSizeT() >= size_) {
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
    return accumulate<double>(0., [](double total, auto const &kv){
        auto const& [basisVector, sparseComplex] = kv;
        return total + std::norm(sparseComplex.value);
    });
}

[[nodiscard]] std::vector<std::complex<double>> SparseArray::toVector() const {
    auto result = std::vector<std::complex<double>>(size_, 0.);
    for (auto const &[basisVector, sparseComplex] : data_) {
        result[basisVector.toSizeT()] = sparseComplex.value;
    }
    return result;
}

void SparseArray::cleanUpZeros() {
    absl::erase_if(data_, [](auto const &kv) {
        auto const &[_, sparseComplex] = kv;
        return isNull(sparseComplex.value);
    });
    zeroCounter_ = 0;
}

}  // namespace qx::core
