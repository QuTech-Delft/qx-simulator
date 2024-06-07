#include "qx/SparseArray.hpp"


namespace qx::core {

SparseArray::SparseArray(std::size_t s)
    : size(s)
{}

[[nodiscard]] std::size_t SparseArray::getSize() const {
    return size;
}

[[nodiscard]] std::vector<std::complex<double>> SparseArray::testToVector() const {
    std::vector<std::complex<double>> result(getSize(), 0);

    for (auto const &kv : *this) {
        result[kv.first.toSizeT()] = kv.second;
    }

    return result;
}

[[nodiscard]] SparseArray::Iterator SparseArray::begin() const {
    return data.cbegin();
}

[[nodiscard]] SparseArray::Iterator SparseArray::end() const {
    return data.cend();
}


void SparseArray::set(BasisVector index, std::complex<double> value) {
#ifndef NDEBUG
    if (index.toSizeT() >= size) {
        throw std::runtime_error("SparseArray::set index out of bounds");
    }
#endif

    if (std::abs(value) < config::EPS) {
        return;
    }

    data.try_emplace(index, value);
}

void SparseArray::clear() {
    data.clear();
}

SparseArray &SparseArray::operator*=(double d) {
    std::for_each(data.begin(), data.end(),
                  [d](auto &kv) { kv.second *= d; });
    return *this;
}

void SparseArray::cleanupZeros() {
    absl::erase_if(data, [](auto const &kv) { return !isNotNull(kv.second); });
    zeroCounter = 0;
}

}  // namespace qx::core
