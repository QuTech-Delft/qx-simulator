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
    Matrix empty_matrix{};
    EXPECT_THAT([&empty_matrix]() { DenseUnitaryMatrix test{ empty_matrix }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is empty"));

    Matrix non_square_matrix{
        Row{ 1, 0 }
    };
    EXPECT_THAT([&non_square_matrix]() { DenseUnitaryMatrix test{ non_square_matrix }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not square"));

    Matrix non_unitary_1{
        Row{ 1,   0 },
        Row{ 0, 1.1 }
    };
    EXPECT_THAT([&non_unitary_1]() { DenseUnitaryMatrix test{ non_unitary_1 }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    Matrix non_unitary_2{
        Row{ 0, 0 },
        Row{ 0, 1 }
    };
    EXPECT_THAT([&non_unitary_2]() { DenseUnitaryMatrix test{ non_unitary_2 }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));

    Matrix non_unitary_3{
        Row{ 1i,  0,  0 },
        Row{  0, 1i, 1i },
        Row{  0,  1,  0 }
    };
    EXPECT_THAT([&non_unitary_3]() { DenseUnitaryMatrix test{ non_unitary_3 }; },
        ::testing::ThrowsMessage<std::runtime_error>("matrix is not unitary"));
}

TEST(dense_unitary_matrix_test, identity) {
    DenseUnitaryMatrix expected_identity{ Matrix{
        Row{ 1, 0, 0, 0, 0 },
        Row{ 0, 1, 0, 0, 0 },
        Row{ 0, 0, 1, 0, 0 },
        Row{ 0, 0, 0, 1, 0 },
        Row{ 0, 0, 0, 0, 1 }
    }};
    EXPECT_EQ(DenseUnitaryMatrix::identity(5), expected_identity);
}

TEST(dense_unitary_matrix_test, dagger) {
    DenseUnitaryMatrix m{ Matrix{
        Row{  1 / std::numbers::sqrt2,   1 / std::numbers::sqrt2 },
        Row{ 1i / std::numbers::sqrt2, -1i / std::numbers::sqrt2 }
    }};
    DenseUnitaryMatrix m_dag{ Matrix{
        Row{ 1 / std::numbers::sqrt2, -1i / std::numbers::sqrt2 },
        Row{ 1 / std::numbers::sqrt2,  1i / std::numbers::sqrt2 }
    }};
    EXPECT_EQ(m.dagger(), m_dag);
}

TEST(dense_unitary_matrix_test, inverse) {
    EXPECT_EQ(gates::RX(gates::PI/4).inverse(), gates::RX(-gates::PI/4));
    EXPECT_EQ(gates::S.inverse(), gates::SDAG);
    EXPECT_EQ(gates::T.inverse(), gates::TDAG);
    EXPECT_EQ(gates::X90.inverse(), gates::RX(-gates::PI/2));
}

TEST(dense_unitary_matrix_test, power) {
    EXPECT_THAT([]() { gates::H.power(1.5); },
        ::testing::ThrowsMessage<std::runtime_error>("unimplemented: matrix power with a non-integer exponent"));
    EXPECT_EQ(gates::H.power(2), gates::IDENTITY);
    EXPECT_EQ(gates::X.power(2), gates::IDENTITY);
    DenseUnitaryMatrix minus_identity{ Matrix{
        Row{ -1,  0 },
        Row{  0, -1 }
    }};
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
