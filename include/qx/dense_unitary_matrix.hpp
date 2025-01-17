#pragma once

#include <array>
#include <complex>  // conj
#include <cstdint>  // size_t
#include <cstdlib>  // abs
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/iota.hpp>
#include <stdexcept>  // runtime_error

#include "qx/core.hpp"  // QubitIndex, is_null

namespace qx::core {

template <std::size_t N>
using Matrix = std::array<std::array<std::complex<double>, N>, N>;

template <std::size_t N>
class DenseUnitaryMatrix {
public:
    explicit constexpr DenseUnitaryMatrix(const Matrix<N>& m)
    : DenseUnitaryMatrix{ m, true } {}

    [[nodiscard]] constexpr std::complex<double>& at(std::size_t i, std::size_t j) {
        return matrix[i][j];
    }
    
    [[nodiscard]] constexpr const std::complex<double>& at(std::size_t i, std::size_t j) const {
        return matrix[i][j];
    }

    constexpr bool operator==(const DenseUnitaryMatrix<N>& other) const {
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                if (not is_null(matrix[i][j] - other.matrix[i][j])) {
                    return false;
                }
            }
        }
        return true;
    }

    constexpr DenseUnitaryMatrix<N> operator*(const DenseUnitaryMatrix<N>& other) const {
        Matrix<N> m{};
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = 0;
                for (std::size_t k = 0; k < N; ++k) {
                    m[i][j] += matrix[i][k] * other.matrix[k][j];
                }
            }
        }
        return DenseUnitaryMatrix{ m, false };
    }

    constexpr DenseUnitaryMatrix<N*2> control() const {
        auto ret{ DenseUnitaryMatrix<N*2>::identity() };
        std::size_t first_index = N;
        std::size_t last_index = N*2;
        for (std::size_t i = first_index; i < last_index; ++i) {
            for (std::size_t j = first_index; j < last_index; ++j) {
                ret.at(i, j) = matrix[i - first_index][j - first_index];
            }
        }
        return ret;
    }

    constexpr DenseUnitaryMatrix<N> dagger() const {
        Matrix<N> m{};
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = std::conj(matrix[j][i]);
            }
        }
        return DenseUnitaryMatrix{ m, false };
    }

    static constexpr DenseUnitaryMatrix<N> identity() {
        Matrix<N> m{};
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = (i == j) ? 1 : 0;
            }
        }
        return DenseUnitaryMatrix{ m, false };
    }

    constexpr DenseUnitaryMatrix<N> inverse() const {
        return dagger();
    }

    constexpr DenseUnitaryMatrix<N> power(int exponent) const {
        // A^ 0 = I
        if (exponent == 0) {
            return identity();
        }
        // A^ n = A    * A      ... * A    (n times)
        // A^-n = A^-1 * A^-1 * ... * A^-1 (n times)
        const auto& init{ (exponent > 0) ? *this : inverse() };
        return ranges::accumulate(ranges::views::iota(1, std::abs(exponent)), init, [&init](auto& acc, auto) {
            acc = acc * init;
            return acc;
        });
    }

private:
    constexpr DenseUnitaryMatrix(const Matrix<N>& m, bool is_unitary_check)
    : matrix{ m } {
        if (is_unitary_check) {
            check_is_unitary();
        }
    }

    constexpr void check_is_unitary() const {
        if (*this * dagger() != identity()) {
            throw std::runtime_error{ "matrix is not unitary" };
        }
    }

    Matrix<N> matrix;
};

template <std::size_t N>
using matrix_t = core::DenseUnitaryMatrix<1 << N>;
template <std::size_t N>
using operands_t = std::array<core::QubitIndex, N>;

}  // namespace qx::core
