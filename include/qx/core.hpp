#pragma once

#include <fmt/ostream.h>

#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <complex>
#include <cstdint>  // size_t, uint32_t
#include <cstdlib>  // abs
#include <string>

#include "qx/compile_time_configuration.hpp"  // EPSILON

namespace qx::core {

struct Complex {
    double real = 0;
    double imag = 0;
    double norm = 0;
    bool operator==(const Complex& other) const {
        return std::abs(real - other.real) < config::EPSILON && std::abs(imag - other.imag) < config::EPSILON &&
            std::abs(norm - other.norm) < config::EPSILON;
    }
    auto operator<=>(const Complex& other) const = default;
};

struct QubitIndex {
    std::size_t value;
};

struct BitIndex {
    std::size_t value;
};

using BasisVector = boost::dynamic_bitset<uint32_t>;
using MeasurementRegister = boost::dynamic_bitset<uint32_t>;
using BitMeasurementRegister = boost::dynamic_bitset<uint32_t>;
using PairBasisVectorStringComplex = std::pair<std::string, std::complex<double>>;

[[nodiscard]] inline constexpr bool is_not_null(std::complex<double> c) {
#if defined(_MSC_VER)
    return c.real() > config::EPSILON || -c.real() > config::EPSILON || c.imag() > config::EPSILON ||
        -c.imag() > config::EPSILON;
#else
    return std::abs(c.real()) > config::EPSILON || std::abs(c.imag()) > config::EPSILON;
#endif
}

[[nodiscard]] inline constexpr bool is_null(std::complex<double> c) {
    return not is_not_null(c);
}

// Convert to string, then return the rightmost n bits
// Notice the least significant bits go to the right
[[nodiscard]] inline std::string to_substring(boost::dynamic_bitset<uint32_t> const& bs, size_t n) {
    auto ret = std::string{};
    boost::to_string(bs, ret);
    return ret.substr(ret.size() - n, ret.size());
}

}  // namespace qx::core

template <>
struct fmt::formatter<boost::dynamic_bitset<uint32_t>> : fmt::ostream_formatter {};
