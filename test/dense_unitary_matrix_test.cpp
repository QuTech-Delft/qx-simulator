#include "qx/dense_unitary_matrix.hpp"

#include <gmock/gmock.h>  // ThrowsMessage
#include <gtest/gtest.h>

#include <numbers>
#include <stdexcept>  // runtime_error

namespace qx::core {

using namespace std::complex_literals;

// clang-format  off

TEST(dense_unitary_matrix_test, identity) {
    DenseUnitaryMatrix<5> expected_identity(
        { { { 1, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 1, 0 }, { 0, 0, 0, 0, 1 } } });

    EXPECT_EQ(DenseUnitaryMatrix<5>::identity(), expected_identity);
}

TEST(dense_unitary_matrix_test, reject_non_unitary_matrices) {
    DenseUnitaryMatrix<2>::Matrix non_unitary_1({ { { 1, 0 }, { 0, 1.1 } } });

    EXPECT_THAT([&non_unitary_1]() { DenseUnitaryMatrix<2> test(non_unitary_1); },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    DenseUnitaryMatrix<2>::Matrix non_unitary_2({ { { 0, 0 }, { 0, 1 } } });

    EXPECT_THAT([&non_unitary_2]() { DenseUnitaryMatrix<2> test(non_unitary_2); },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    DenseUnitaryMatrix<3>::Matrix non_unitary_3({ { { 1i, 0, 0 }, { 0, 1i, 1i }, { 0, 1, 0 } } });

    EXPECT_THAT([&non_unitary_3]() { DenseUnitaryMatrix<3> test(non_unitary_3); },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));
}

TEST(dense_unitary_matrix_test, dagger) {
    DenseUnitaryMatrix<2> m({ { { 1 / std::numbers::sqrt2, 1 / std::numbers::sqrt2 },
        { 1i / std::numbers::sqrt2, -1i / std::numbers::sqrt2 } } });

    DenseUnitaryMatrix<2> m_dag({ { { 1 / std::numbers::sqrt2, -1i / std::numbers::sqrt2 },
        { 1 / std::numbers::sqrt2, 1i / std::numbers::sqrt2 } } });

    EXPECT_EQ(m.dagger(), m_dag);
}

// clang-format on

}  // namespace qx::core
