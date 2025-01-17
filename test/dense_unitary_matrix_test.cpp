#include "qx/dense_unitary_matrix.hpp"
#include "qx/gates.hpp"

#include <gmock/gmock.h>  // ThrowsMessage
#include <gtest/gtest.h>

#include <numbers>
#include <stdexcept>  // runtime_error

namespace qx::core {

using namespace std::complex_literals;

// clang-format off

TEST(dense_unitary_matrix_test, reject_non_unitary_matrices) {
    Matrix<2> non_unitary_1{{
        { 1,   0 },
        { 0, 1.1 }
    }};
    EXPECT_THAT([&non_unitary_1]() { DenseUnitaryMatrix<2> test{ non_unitary_1 }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    Matrix<2> non_unitary_2{{
        { 0, 0 },
        { 0, 1 }
    }};
    EXPECT_THAT([&non_unitary_2]() { DenseUnitaryMatrix<2> test{ non_unitary_2 }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    Matrix<3> non_unitary_3{{
        { 1i,  0,  0 },
        {  0, 1i, 1i },
        {  0,  1,  0 }
    }};
    EXPECT_THAT([&non_unitary_3]() { DenseUnitaryMatrix<3> test{ non_unitary_3 }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));
}

TEST(dense_unitary_matrix_test, identity) {
    DenseUnitaryMatrix<5> expected_identity{{{
        { 1, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 1 }
    }}};
    EXPECT_EQ(DenseUnitaryMatrix<5>::identity(), expected_identity);
}

TEST(dense_unitary_matrix_test, dagger) {
    DenseUnitaryMatrix<2> m{{{
        {  1 / std::numbers::sqrt2,   1 / std::numbers::sqrt2 },
        { 1i / std::numbers::sqrt2, -1i / std::numbers::sqrt2 }
    }}};
    DenseUnitaryMatrix<2> m_dag{{{
        { 1 / std::numbers::sqrt2, -1i / std::numbers::sqrt2 },
        { 1 / std::numbers::sqrt2,  1i / std::numbers::sqrt2 }
    }}};
    EXPECT_EQ(m.dagger(), m_dag);
}

TEST(dense_unitary_matrix_test, inverse) {
    EXPECT_EQ(gates::RX(gates::PI/4).inverse(), gates::RX(-gates::PI/4));
    EXPECT_EQ(gates::S.inverse(), gates::SDAG);
    EXPECT_EQ(gates::T.inverse(), gates::TDAG);
    EXPECT_EQ(gates::X90.inverse(), gates::RX(-gates::PI/2));
}

TEST(dense_unitary_matrix_test, power) {
    EXPECT_EQ(gates::H.power(2), gates::IDENTITY);
    EXPECT_EQ(gates::X.power(2), gates::IDENTITY);
    DenseUnitaryMatrix<2> minus_identity{{{
        { -1,  0 },
        {  0, -1 }
    }}};
    EXPECT_EQ(gates::RX(gates::PI).power(2), minus_identity);
    EXPECT_EQ(gates::S.power(2), gates::Z);
    EXPECT_EQ(gates::T.power(4), gates::Z);
}

TEST(dense_unitary_matrix_test, control) {
    EXPECT_EQ(gates::X.control(), gates::CNOT);
    EXPECT_EQ(gates::Z.control(), gates::CZ);
    EXPECT_EQ(gates::X.control().control(), gates::TOFFOLI);
}

// clang-format on

}  // namespace qx::core
