#pragma once

#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <cstdint>  // size_t, uint32_t
#include <cstdlib>  // abs
#include <complex>
#include <fmt/ostream.h>

#include "qx/compile_time_configuration.hpp" // EPS, MAX_QUBIT_NUMBER
#include "qx/utils.hpp"

namespace qx::core {

struct Complex {
    double real = 0;
    double imag = 0;
    double norm = 0;
    bool operator==(const Complex &other) const {
        return std::abs(real - other.real) < config::EPS &&
               std::abs(imag - other.imag) < config::EPS &&
               std::abs(norm - other.norm) < config::EPS;
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

inline constexpr bool isNotNull(std::complex<double> c) {
#if defined(_MSC_VER)
    return
        c.real() > config::EPS || -c.real() > config::EPS ||
        c.imag() > config::EPS || -c.imag() > config::EPS;
#else
    return std::abs(c.real()) > config::EPS || std::abs(c.imag()) > config::EPS;
#endif
}

inline constexpr bool isNull(std::complex<double> c) {
    return not isNotNull(c);
}

}  // namespace qx::core

template <> struct fmt::formatter<qx::core::BitMeasurementRegister> : fmt::ostream_formatter {};
