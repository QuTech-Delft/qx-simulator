#include "qx/dense_unitary_matrix.hpp"

#include <algorithm>  // copy
#include <unsupported/Eigen/MatrixFunctions>

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
    const auto& eigen_matrix = to_eigen_matrix(matrix_);
    Eigen::MatrixPower<Eigen::MatrixXcd> eigen_power_matrix(eigen_matrix);
    auto ret = from_eigen_matrix(eigen_power_matrix(exponent));
    return DenseUnitaryMatrix{ ret, true };
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

// Matrix is in row-major format, i.e., it stores a matrix as a vector of rows
// Eigen uses by default a column-major format, i.e., it stores a matrix as a vector of columns
// Notice, however, that this only means that:
// - When going from Matrix to Eigen, we set Eigen column vectors with Matrix row values
// - When going from Eigen to Matrix, we set Matrix row vectors with Eigen column values
[[nodiscard]] Matrix DenseUnitaryMatrix::from_eigen_matrix(const Eigen::MatrixXcd& eigen_matrix) const {
    Matrix ret(N, Row(N));
    for (std::size_t i = 0; i < N; ++i) {
        std::copy(eigen_matrix.col(i).begin(), eigen_matrix.col(i).end(), ret[i].begin());
    }
    return ret;
}

[[nodiscard]] Eigen::MatrixXcd DenseUnitaryMatrix::to_eigen_matrix(const Matrix& matrix) const {
    Eigen::MatrixXcd ret(N, N);
    for (std::size_t i = 0; i < N; ++i) {
        ret.col(i) = Eigen::Map<const Eigen::VectorXcd>(matrix[i].data(), N);
    }
    return ret;
}

}  // namespace qx::core
