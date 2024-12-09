#pragma once

#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <cstdint>  // size_t, uint32_t
#include <cstdlib>  // abs
#include <complex>
#include <fmt/ostream.h>
#include <string_view>

#include "qx/compile_time_configuration.hpp" // EPSILON, MAX_QUBIT_NUMBER
#include "qx/utils.hpp"

namespace qx::core {

struct Complex {
    double real = 0;
    double imag = 0;
    double norm = 0;
    bool operator==(const Complex &other) const {
        return std::abs(real - other.real) < config::EPSILON &&
               std::abs(imag - other.imag) < config::EPSILON &&
               std::abs(norm - other.norm) < config::EPSILON;
    }
    auto operator<=>(const Complex &other) const = default;
};

struct QubitIndex {
    std::size_t value;
};

struct BitIndex {
    std::size_t value;
};

using BasisVector = utils::Bitset<config::MAX_QUBIT_NUMBER>;

using BitMeasurementRegister = boost::dynamic_bitset<uint32_t>;

using PairBasisVectorStringComplex = std::pair<std::string_view, std::complex<double>>;

inline constexpr bool is_not_null(std::complex<double> c) {
#if defined(_MSC_VER)
    return
        c.real() > config::EPSILON || -c.real() > config::EPSILON ||
        c.imag() > config::EPSILON || -c.imag() > config::EPSILON;
#else
    return std::abs(c.real()) > config::EPSILON || std::abs(c.imag()) > config::EPSILON;
#endif
}

inline constexpr bool is_null(std::complex<double> c) {
    return not is_not_null(c);
}

}  // namespace qx::core

template <> struct fmt::formatter<qx::core::BitMeasurementRegister> : fmt::ostream_formatter {};
