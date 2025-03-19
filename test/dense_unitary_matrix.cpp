#include "qx/dense_unitary_matrix.hpp"

#include <gmock/gmock.h>  // ThrowsMessage
#include <gtest/gtest.h>

#include <complex>  // exp
#include <stdexcept>  // runtime_error

#include "qx/gates.hpp"

namespace qx::core {

using namespace std::complex_literals;

using gates::PI;
using gates::SQRT_2;

// clang-format off

TEST(dense_unitary_matrix_test, reject_non_unitary_matrices) {
    Matrix empty_matrix{};
    EXPECT_THAT([&empty_matrix]() { DenseUnitaryMatrix test{ std::move(empty_matrix) }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is empty"));

    Matrix non_square_matrix{
        { 1, 0 }
    };
    EXPECT_THAT([&non_square_matrix]() { DenseUnitaryMatrix test{ std::move(non_square_matrix) }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not square"));

    Matrix non_unitary_1{
        { 1,   0 },
        { 0, 1.1 }
    };
    EXPECT_THAT([&non_unitary_1]() { DenseUnitaryMatrix test{ std::move(non_unitary_1) }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    Matrix non_unitary_2{
        { 0, 0 },
        { 0, 1 }
    };
    EXPECT_THAT([&non_unitary_2]() { DenseUnitaryMatrix test{ std::move(non_unitary_2) }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    Matrix non_unitary_3{
        { 1i,  0,  0 },
        {  0, 1i, 1i },
        {  0,  1,  0 }
    };
    EXPECT_THAT([&non_unitary_3]() { DenseUnitaryMatrix test{ std::move(non_unitary_3) }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));
}

TEST(dense_unitary_matrix_test, identity) {
    DenseUnitaryMatrix expected_identity{ Matrix{
        { 1, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 1 }
    }};
    EXPECT_EQ(DenseUnitaryMatrix::identity(5), expected_identity);
}

TEST(dense_unitary_matrix_test, dagger) {
    DenseUnitaryMatrix matrix{
        Matrix{
            {  1 / gates::SQRT_2,   1 / gates::SQRT_2 },
            { 1i / gates::SQRT_2, -1i / gates::SQRT_2 }
        }
    };
    DenseUnitaryMatrix matrix_dagger{
        Matrix{
            { 1 / gates::SQRT_2, -1i / gates::SQRT_2 },
            { 1 / gates::SQRT_2,  1i / gates::SQRT_2 }
        }
    };
    EXPECT_EQ(matrix.dagger(), matrix_dagger);
}

TEST(dense_unitary_matrix_test, inverse) {
    EXPECT_EQ(gates::RX(PI/4).inverse(), gates::RX(-PI/4));
    EXPECT_EQ(gates::S.inverse(), gates::SDAG);
    EXPECT_EQ(gates::T.inverse(), gates::TDAG);
    EXPECT_EQ(gates::X90.inverse(), gates::RX(-PI/2));
}

TEST(dense_unitary_matrix_test, power_integer) {
    EXPECT_EQ(gates::H.power(2), gates::IDENTITY);
    EXPECT_EQ(gates::X.power(2), gates::IDENTITY);
    DenseUnitaryMatrix minus_identity{ Matrix{
        { -1,  0 },
        {  0, -1 }
    }};
    EXPECT_EQ(gates::RX(PI).power(2), minus_identity);
    EXPECT_EQ(gates::S.power(2), gates::Z);
    EXPECT_EQ(gates::T.power(4), gates::Z);
}

// The matrices defined here as powers of a default gate are valid roots computed by Eigen
// For example, for sqrt(X), we get a unitary matrix equal to X90 up to a global phase
TEST(dense_unitary_matrix_test, power_fractional) {
    const auto& x_power_1_2 = DenseUnitaryMatrix{
        Matrix{
            {  std::exp(std::complex<double>(0, PI/4.)) / SQRT_2, std::exp(std::complex<double>(0, -PI/4.)) / SQRT_2 },
            { std::exp(std::complex<double>(0, -PI/4.)) / SQRT_2,  std::exp(std::complex<double>(0, PI/4.)) / SQRT_2 }
        }
    };
    EXPECT_EQ(gates::X.power(1./2), x_power_1_2);  // X90

    const auto& x_power_m1_2 = DenseUnitaryMatrix{
        Matrix{
            { std::exp(std::complex<double>(0, -PI/4.)) / SQRT_2,  std::exp(std::complex<double>(0, PI/4.)) / SQRT_2 },
            {  std::exp(std::complex<double>(0, PI/4.)) / SQRT_2, std::exp(std::complex<double>(0, -PI/4.)) / SQRT_2 }
        }
    };
    EXPECT_EQ(gates::X.power(-1./2), x_power_m1_2);  // MX90

    EXPECT_EQ(gates::Z.power(1./2), gates::S);
    EXPECT_EQ(gates::Z.power(-1./2), gates::SDAG);
    EXPECT_EQ(gates::Z.power(1./4), gates::T);
    EXPECT_EQ(gates::Z.power(-1./4), gates::TDAG);

    const auto& s_power_1_2 = DenseUnitaryMatrix{
        Matrix{
            { 1,                                         0 },
            { 0, std::exp(std::complex<double>(0, PI / 4)) }
        }
    };
    EXPECT_EQ(gates::S.power(1./2), s_power_1_2);

    const auto& z_power_1_3 = DenseUnitaryMatrix{
        Matrix{
            { 1,                                         0 },
            { 0, std::exp(std::complex<double>(0, PI / 3)) }
        }
    };
    EXPECT_EQ(gates::Z.power(1./3), z_power_1_3);

    EXPECT_EQ(gates::Z.power(1./4).power(4).power(1./3).power(3).power(1./2).power(2), gates::Z);
}

TEST(dense_unitary_matrix_test, control) {
    EXPECT_EQ(gates::X.control(), gates::CNOT);
    EXPECT_EQ(gates::Z.control(), gates::CZ);
    EXPECT_EQ(gates::X.control().control(), gates::TOFFOLI);
}

// clang-format on

}  // namespace qx::core
