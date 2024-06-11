#include "qx/SparseArray.hpp"

#include <complex>


namespace qx::core {

SparseArrayError::SparseArrayError(const std::string &message)
    : std::runtime_error{ message }
{}

SparseComplex::SparseComplex(std::complex<double> value) {
    value = value;
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
    : size_(s)
{}

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
        return !isNotNull(sparseComplex.value);
    });
    zeroCounter_ = 0;
}

}  // namespace qx::core
