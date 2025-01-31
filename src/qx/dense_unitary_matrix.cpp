#include "qx/dense_unitary_matrix.hpp"

#include <algorithm>  // copy
#include <unsupported/Eigen/MatrixFunctions>

namespace qx::core {

DenseUnitaryMatrix::DenseUnitaryMatrix(const Matrix& m)
: DenseUnitaryMatrix{ m, true } {}

[[nodiscard]] std::complex<double>& DenseUnitaryMatrix::at(std::size_t i, std::size_t j) {
    return matrix.at(i).at(j);
}

[[nodiscard]] const std::complex<double>& DenseUnitaryMatrix::at(std::size_t i, std::size_t j) const {
    return matrix.at(i).at(j);
}

bool DenseUnitaryMatrix::operator==(const DenseUnitaryMatrix& other) const {
    if (N != other.N) {
        return false;
    }
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            if (not is_null(matrix[i][j] - other.matrix[i][j])) {
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
    Matrix m(N, Row(N));
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

/* static */ DenseUnitaryMatrix DenseUnitaryMatrix::identity(size_t M) {
    Matrix m(M, Row(M));
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            m[i][j] = (i == j) ? 1 : 0;
        }
    }
    return DenseUnitaryMatrix{ m, false };
}

DenseUnitaryMatrix DenseUnitaryMatrix::dagger() const {
    Matrix m(N, Row(N));
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            m[i][j] = std::conj(matrix[j][i]);
        }
    }
    return DenseUnitaryMatrix{ m, false };
}

DenseUnitaryMatrix DenseUnitaryMatrix::inverse() const {
    return dagger();
}

DenseUnitaryMatrix DenseUnitaryMatrix::power(double exponent) const {
    const auto& eigen_matrix = to_eigen_matrix();
    Eigen::MatrixPower<Eigen::MatrixXcd> eigen_power_matrix(eigen_matrix);
    auto ret = from_eigen_matrix(eigen_power_matrix(exponent));
    return DenseUnitaryMatrix{ ret, false };
}

DenseUnitaryMatrix DenseUnitaryMatrix::control() const {
    auto ret{ DenseUnitaryMatrix::identity(N * 2) };
    std::size_t first_index = N;
    std::size_t last_index = N * 2;
    for (std::size_t i = first_index; i < last_index; ++i) {
        for (std::size_t j = first_index; j < last_index; ++j) {
            ret.at(i, j) = matrix[i - first_index][j - first_index];
        }
    }
    return ret;
}

DenseUnitaryMatrix::DenseUnitaryMatrix(const Matrix& m, bool is_unitary_check)
: matrix{ m }
, N{ m.size() } {
    if (is_unitary_check) {
        check_is_unitary();
    }
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
    if (!std::all_of(matrix.begin(), matrix.end(), [this](const auto& row) { return row.size() == N; })) {
        throw std::runtime_error{ "matrix is not square" };
    }
}

[[nodiscard]] Matrix DenseUnitaryMatrix::from_eigen_matrix(const Eigen::MatrixXcd& eigen_matrix) const {
    Matrix ret(N, Row(N));
    for (std::size_t i = 0; i < N; ++i) {
        std::copy(eigen_matrix.col(i).begin(), eigen_matrix.col(i).end(), ret[i].begin());
    }
    return ret;
}

[[nodiscard]] Eigen::MatrixXcd DenseUnitaryMatrix::to_eigen_matrix() const {
    Eigen::MatrixXcd ret(N, N);
    for (std::size_t i = 0; i < N; ++i) {
        ret.col(i) = Eigen::Map<const Eigen::VectorXcd>(matrix[i].data(), N);
    }
    return ret;
}

}  // namespace qx::core
