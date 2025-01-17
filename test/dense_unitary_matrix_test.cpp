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

TEST(dense_unitary_matrix_test, cofactor) {
    Matrix<4> m{{
        {  5, -2,  2, 7 },
        {  1,  0,  0, 3 },
        { -3,  1,  5, 0 },
        {  3, -1, -9, 4 },
    }};
    Matrix<3> expected{{
        {  5,  2, 7 },
        { -3,  5, 0 },
        {  3, -9, 4 },
    }};
    EXPECT_EQ(cofactor(m, 1, 1), expected);
}

TEST(dense_unitary_matrix_test, determinant) {
    Matrix<1> m1{{
        { 5 }
    }};
    EXPECT_EQ(determinant(m1), std::complex<double>(5));

    Matrix<3> m3{{
        {  1,  0, 3 },
        { -3,  5, 0 },
        {  3, -9, 4 },
    }};
    EXPECT_EQ(determinant(m3), std::complex<double>(56));
}

TEST(dense_unitary_matrix_test, adjoint) {
    Matrix<4> m{{
        {  5, -2,  2, 7 },
        {  1,  0,  0, 3 },
        { -3,  1,  5, 0 },
        {  3, -1, -9, 4 },
    }};
    Matrix<4> expected{{
        { -12,  76, -60, -36 },
        { -56, 208, -82, -58 },
        {   4,   4,  -2, -10 },
        {   4,   4,  20,  12 },
    }};
    EXPECT_EQ(adjoint(m), expected);
}

TEST(dense_unitary_matrix_test, inverse) {
}

TEST(dense_unitary_matrix_test, power) {

}

TEST(dense_unitary_matrix_test, control) {
    EXPECT_EQ(gates::X.control(), gates::CNOT);
    EXPECT_EQ(gates::Z.control(), gates::CZ);
    EXPECT_EQ(gates::X.control().control(), gates::TOFFOLI);
}

// clang-format on

}  // namespace qx::core
