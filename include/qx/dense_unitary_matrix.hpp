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
    explicit DenseUnitaryMatrix(const Matrix& m);

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
    DenseUnitaryMatrix(const Matrix& m, bool is_unitary_check);

    void check_is_unitary() const;
    void check_is_square() const;

    Matrix from_eigen_matrix(const Eigen::MatrixXcd& eigen_matrix) const;
    Eigen::MatrixXcd to_eigen_matrix() const;

    Matrix matrix;
    size_t N;  // matrix size
};

using matrix_t = core::DenseUnitaryMatrix;
using operands_t = std::vector<core::QubitIndex>;

}  // namespace qx::core
