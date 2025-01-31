#include "qx/dense_unitary_matrix.hpp"

#include <gmock/gmock.h>  // ThrowsMessage
#include <gtest/gtest.h>

#include <numbers>
#include <stdexcept>  // runtime_error

#include "qx/gates.hpp"

namespace qx::core {

using namespace std::complex_literals;

// clang-format off

TEST(dense_unitary_matrix_test, reject_non_unitary_matrices) {
    Matrix empty_matrix{};
    EXPECT_THAT([&empty_matrix]() { DenseUnitaryMatrix test{ empty_matrix }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is empty"));

    Matrix non_square_matrix{
        { 1, 0 }
    };
    EXPECT_THAT([&non_square_matrix]() { DenseUnitaryMatrix test{ non_square_matrix }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not square"));

    Matrix non_unitary_1{
        { 1,   0 },
        { 0, 1.1 }
    };
    EXPECT_THAT([&non_unitary_1]() { DenseUnitaryMatrix test{ non_unitary_1 }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    Matrix non_unitary_2{
        { 0, 0 },
        { 0, 1 }
    };
    EXPECT_THAT([&non_unitary_2]() { DenseUnitaryMatrix test{ non_unitary_2 }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    Matrix non_unitary_3{
        { 1i,  0,  0 },
        {  0, 1i, 1i },
        {  0,  1,  0 }
    };
    EXPECT_THAT([&non_unitary_3]() { DenseUnitaryMatrix test{ non_unitary_3 }; },
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
    DenseUnitaryMatrix m{ Matrix{
        {  1 / std::numbers::sqrt2,   1 / std::numbers::sqrt2 },
        { 1i / std::numbers::sqrt2, -1i / std::numbers::sqrt2 }
    }};
    DenseUnitaryMatrix m_dag{ Matrix{
        { 1 / std::numbers::sqrt2, -1i / std::numbers::sqrt2 },
        { 1 / std::numbers::sqrt2,  1i / std::numbers::sqrt2 }
    }};
    EXPECT_EQ(m.dagger(), m_dag);
}

TEST(dense_unitary_matrix_test, inverse) {
    EXPECT_EQ(gates::RX(gates::PI/4).inverse(), gates::RX(-gates::PI/4));
    EXPECT_EQ(gates::S.inverse(), gates::SDAG);
    EXPECT_EQ(gates::T.inverse(), gates::TDAG);
    EXPECT_EQ(gates::X90.inverse(), gates::RX(-gates::PI/2));
}

TEST(dense_unitary_matrix_test, power_integer) {
    EXPECT_EQ(gates::H.power(2), gates::IDENTITY);
    EXPECT_EQ(gates::X.power(2), gates::IDENTITY);
    DenseUnitaryMatrix minus_identity{ Matrix{
        { -1,  0 },
        {  0, -1 }
    }};
    EXPECT_EQ(gates::RX(gates::PI).power(2), minus_identity);
    EXPECT_EQ(gates::S.power(2), gates::Z);
    EXPECT_EQ(gates::T.power(4), gates::Z);
}

TEST(dense_unitary_matrix_test, power_fractional) {
    EXPECT_EQ(gates::Z.power(1./2), gates::S);

    const auto& s_power_1_2 = DenseUnitaryMatrix{
        Matrix{
            { 1,                        0 },
            { 0, 0.70710678 + 0.70710678i }
        }
    };
    EXPECT_EQ(gates::S.power(1./2), s_power_1_2);

    const auto& z_power_1_3 = DenseUnitaryMatrix{
        Matrix{
            { 1,                0 },
            { 0, 0.5 + 0.8660254i }
        }
    };
    EXPECT_EQ(gates::Z.power(1./3), z_power_1_3);
}

TEST(dense_unitary_matrix_test, control) {
    EXPECT_EQ(gates::X.control(), gates::CNOT);
    EXPECT_EQ(gates::Z.control(), gates::CZ);
    EXPECT_EQ(gates::X.control().control(), gates::TOFFOLI);
}

// clang-format on

}  // namespace qx::core
