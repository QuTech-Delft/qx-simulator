#pragma once

#include <cstdint>  // size_t
#include <cstdlib>  // abs
#include <complex>

#include "qx/CompileTimeConfiguration.hpp"  // EPS, MAX_QUBIT_NUMBER
#include "qx/Utils.hpp"


namespace qx::core {

struct QubitIndex {
    std::size_t value;
};

struct BitIndex {
    std::size_t value;
};

using BasisVector = utils::Bitset<config::MAX_QUBIT_NUMBER>;

inline constexpr bool isNotNull(std::complex<double> c) {
#if defined(_MSC_VER)
    return c.real() > config::EPS || -c.real() > config::EPS ||
           c.imag() > config::EPS || -c.imag() > config::EPS;
#else
    return std::abs(c.real()) > config::EPS || std::abs(c.imag()) > config::EPS;
#endif
}

}  // namespace qx::core
