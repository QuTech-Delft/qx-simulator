#include "qx/dense_unitary_matrix.hpp"

namespace qx::core {

DenseUnitaryMatrix::DenseUnitaryMatrix(const Matrix& matrix, bool is_unitary_check)
: matrix_{ matrix }
, N{ matrix.size() } {
    if (is_unitary_check) {
        check_is_unitary();
    }
}

DenseUnitaryMatrix::DenseUnitaryMatrix(Matrix&& matrix, bool is_unitary_check)
: matrix_{ matrix }
, N{ matrix.size() } {
    if (is_unitary_check) {
        check_is_unitary();
    }
}

[[nodiscard]] std::complex<double>& DenseUnitaryMatrix::at(std::size_t i, std::size_t j) {
    return matrix_.at(i).at(j);
}

[[nodiscard]] const std::complex<double>& DenseUnitaryMatrix::at(std::size_t i, std::size_t j) const {
    return matrix_.at(i).at(j);
}

bool DenseUnitaryMatrix::operator==(const DenseUnitaryMatrix& other) const {
    if (N != other.N) {
        return false;
    }
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            if (not is_null(matrix_[i][j] - other.matrix_[i][j])) {
                return false;
            }
        }
    }
    return true;
}

DenseUnitaryMatrix DenseUnitaryMatrix::operator*(const DenseUnitaryMatrix& other) const {
    if (N != other.N) {
        throw std::runtime_error{ "cannot multiply matrices" };
    }
    Matrix matrix(N, Row(N));
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            matrix[i][j] = 0;
            for (std::size_t k = 0; k < N; ++k) {
                matrix[i][j] += matrix_[i][k] * other.matrix_[k][j];
            }
        }
    }
    return DenseUnitaryMatrix{ std::move(matrix), false };
}

/* static */ DenseUnitaryMatrix DenseUnitaryMatrix::identity(size_t M) {
    Matrix matrix(M, Row(M));
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            matrix[i][j] = (i == j) ? 1 : 0;
        }
    }
    return DenseUnitaryMatrix{ std::move(matrix), false };
}

DenseUnitaryMatrix DenseUnitaryMatrix::dagger() const {
    Matrix matrix(N, Row(N));
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            matrix[i][j] = std::conj(matrix_[j][i]);
        }
    }
    return DenseUnitaryMatrix{ std::move(matrix), false };
}

DenseUnitaryMatrix DenseUnitaryMatrix::inverse() const {
    return dagger();
}

DenseUnitaryMatrix DenseUnitaryMatrix::power(double exponent) const {
    int rounded_exponent = static_cast<int>(std::round(exponent));
    if (std::abs(exponent - rounded_exponent) >= config::EPSILON) {
        throw std::runtime_error{ "unimplemented: matrix power with a non-integer exponent" };
    }
    return power(rounded_exponent);
}

DenseUnitaryMatrix DenseUnitaryMatrix::power(int exponent) const {
    // A^ 0 = I
    if (exponent == 0) {
        return identity(N);
    }
    // A^ n = A    * A      ... * A    (n times)
    // A^-n = A^-1 * A^-1 * ... * A^-1 (n times)
    const auto& init{ (exponent > 0) ? *this : inverse() };
    return ranges::accumulate(ranges::views::iota(1, std::abs(exponent)), init, [&init](auto& acc, auto) {
        acc = acc * init;
        return acc;
    });
}

DenseUnitaryMatrix DenseUnitaryMatrix::control() const {
    auto ret{ DenseUnitaryMatrix::identity(N * 2) };
    std::size_t first_index = N;
    std::size_t last_index = N * 2;
    for (std::size_t i = first_index; i < last_index; ++i) {
        for (std::size_t j = first_index; j < last_index; ++j) {
            ret.at(i, j) = matrix_[i - first_index][j - first_index];
        }
    }
    return ret;
}

void DenseUnitaryMatrix::check_is_unitary() const {
    check_is_square();
    if (*this * dagger() != identity(N)) {
        throw std::runtime_error{ "matrix is not unitary" };
    }
}

void DenseUnitaryMatrix::check_is_square() const {
    if (N == 0) {
        throw std::runtime_error{ "matrix is empty" };
    }
    if (!std::all_of(matrix_.begin(), matrix_.end(), [this](const auto& row) { return row.size() == N; })) {
        throw std::runtime_error{ "matrix is not square" };
    }
}

}  // namespace qx::core
