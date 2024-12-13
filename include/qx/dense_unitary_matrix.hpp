#pragma once

#include <array>
#include <complex>  // conj
#include <cstdint>  // size_t
#include <stdexcept>  // runtime_error

#include "qx/core.hpp"  // QubitIndex, is_null

namespace qx::core {

template <std::size_t N>
class DenseUnitaryMatrix {
public:
    using Matrix = std::array<std::array<std::complex<double>, N>, N>;

    static constexpr DenseUnitaryMatrix<N> identity() {
        Matrix m;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = (i == j) ? 1 : 0;
            }
        }

        return DenseUnitaryMatrix(m, false);
    }

    explicit constexpr DenseUnitaryMatrix(Matrix const& m)
    : DenseUnitaryMatrix(m, true) {}

    [[nodiscard]] inline constexpr const std::complex<double>& at(std::size_t i, std::size_t j) const {
        return matrix[i][j];
    }

    constexpr DenseUnitaryMatrix<N> dagger() const {
        Matrix m;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = std::conj(at(j, i));
            }
        }

        return DenseUnitaryMatrix(m, false);
    }

    constexpr bool operator==(DenseUnitaryMatrix<N> const& other) const {
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                if (not is_null(at(i, j) - other.at(i, j))) {
                    return false;
                }
            }
        }
        return true;
    }

    constexpr DenseUnitaryMatrix<N> operator*(DenseUnitaryMatrix<N> const& other) const {
        Matrix m;
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = 0;
                for (std::size_t k = 0; k < N; ++k) {
                    m[i][j] += at(i, k) * other.at(k, j);
                }
            }
        }

        return DenseUnitaryMatrix(m, false);
    }

private:
    constexpr DenseUnitaryMatrix(Matrix const& m, bool is_unitary_check)
    : matrix(m) {
        if (is_unitary_check) {
            check_is_unitary();
        }
    }

    constexpr void check_is_unitary() const {
        if (*this * dagger() != identity()) {
            throw std::runtime_error{ "matrix is not unitary" };
        }
    }

    std::array<std::array<std::complex<double>, N>, N> const matrix;
};

template <std::size_t N>
using matrix_t = core::DenseUnitaryMatrix<1 << N>;
template <std::size_t N>
using operands_t = std::array<core::QubitIndex, N>;

}  // namespace qx::core
