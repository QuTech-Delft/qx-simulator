#pragma once

#include <Eigen/Dense>
#include <algorithm>  // all_of
#include <array>
#include <complex>  // conj
#include <cstdint>  // size_t
#include <cstdlib>  // abs
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/iota.hpp>
#include <stdexcept>  // runtime_error

#include "qx/compile_time_configuration.hpp"  // EPSILON
#include "qx/core.hpp"  // QubitIndex, is_null

namespace qx::core {

using Row = std::vector<std::complex<double>>;
using Matrix = std::vector<Row>;

class DenseUnitaryMatrix {
public:
    ~DenseUnitaryMatrix() = default;
    DenseUnitaryMatrix(const DenseUnitaryMatrix& other) = default;
    DenseUnitaryMatrix(DenseUnitaryMatrix&& other) noexcept = default;
    DenseUnitaryMatrix& operator=(const DenseUnitaryMatrix& other) = default;
    DenseUnitaryMatrix& operator=(DenseUnitaryMatrix&& other) noexcept = default;

public:
    explicit DenseUnitaryMatrix(const Matrix& matrix, bool is_unitary_check = true);
    explicit DenseUnitaryMatrix(Matrix&& matrix, bool is_unitary_check = true);

    [[nodiscard]] std::complex<double>& at(std::size_t i, std::size_t j);
    [[nodiscard]] const std::complex<double>& at(std::size_t i, std::size_t j) const;

    bool operator==(const DenseUnitaryMatrix& other) const;
    DenseUnitaryMatrix operator*(const DenseUnitaryMatrix& other) const;

    static DenseUnitaryMatrix identity(size_t M);

    [[nodiscard]] DenseUnitaryMatrix dagger() const;
    [[nodiscard]] DenseUnitaryMatrix inverse() const;
    [[nodiscard]] DenseUnitaryMatrix power(double exponent) const;
    [[nodiscard]] DenseUnitaryMatrix control() const;

private:
    DenseUnitaryMatrix() = default;

private:
    void check_is_unitary() const;
    void check_is_square() const;

    [[nodiscard]] Matrix from_eigen_matrix(const Eigen::MatrixXcd& eigen_matrix) const;
    [[nodiscard]] Eigen::MatrixXcd to_eigen_matrix(const Matrix& matrix) const;

    Matrix matrix_;
    size_t N;  // matrix size
};

using matrix_t = core::DenseUnitaryMatrix;
using operands_t = std::vector<core::QubitIndex>;

}  // namespace qx::core
