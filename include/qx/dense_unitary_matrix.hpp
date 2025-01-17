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

template <size_t N>
constexpr Matrix<N-1> cofactor(const Matrix<N>& m, size_t src_row_index_to_delete, size_t src_col_index_to_delete)
requires (N > 1) {
    Matrix<N-1> ret{};
    size_t dst_row_index{ 0 };
    for (std::size_t src_row_index = 0; src_row_index < N; ++src_row_index) {
        if (src_row_index != src_row_index_to_delete) {
            size_t dst_col_index{ 0 };
            for (std::size_t src_col_index = 0; src_col_index < N; ++src_col_index) {
                if (src_col_index != src_col_index_to_delete) {
                    ret[dst_row_index][dst_col_index] = m[src_row_index][src_col_index];
                    dst_col_index++;
                }
            }
            dst_row_index++;
        }
    }
    return ret;
}

template <size_t N = 1>
constexpr std::complex<double> determinant(const Matrix<N>& m){
    return m[0][0];
}

// This code is supposed to be used with unitary matrices, which cannot be singular
// Thus, no check for singular matrix is done
template <size_t N>
constexpr std::complex<double> determinant(const Matrix<N>& m) requires (N > 1) {
    std::complex<double> ret{};
    std::complex<double> sign{ 1 };
    for (size_t j = 0; j < N; ++j) {
        ret += sign * m[0][j] * determinant(cofactor(m, 0, j));
        sign *= -1;
    }
    return ret;
}

template <size_t N>
constexpr Matrix<N> adjoint(const Matrix<N>& m) {
    Matrix<N> ret;
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            auto sign = ((i + j) % 2 == 0) ? 1 : -1;
            ret[j][i] = static_cast<std::complex<double>>(sign) * determinant(cofactor(m, i, j));
        }
    }
    return ret;
}

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
        Matrix<N> m{};
        auto det = determinant(*this);
        auto adj = adjoint(m);
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                m[i][j] = adj[i][j] / det;
            }
        }
        return DenseUnitaryMatrix{ m, false };
    }

    constexpr DenseUnitaryMatrix<N> power(int exponent) const {
        // A^ 0 = I
        if (exponent == 0) {
            return identity();
        }
        // A^ n = A    * A      ... * A    (n times)
        // A^-n = A^-1 * A^-1 * ... * A^-1 (n times)
        const auto& init{ (exponent > 0) ? matrix : inverse().matrix };
        const auto& m = ranges::accumulate(ranges::views::iota(1, std::abs(exponent)), init, [&init](auto& acc, auto) {
            acc *= init;
            return acc;
        });
        return DenseUnitaryMatrix{ m, false };
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
