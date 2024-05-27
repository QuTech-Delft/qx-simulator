#include "qx/Core.hpp"

#include <gmock/gmock.h>  // ThrowsMessage
#include <gtest/gtest.h>
#include <stdexcept>  // runtime_error


namespace qx::core {

using namespace std::complex_literals;

TEST(dense_unitary_matrix_test, identity) {
    DenseUnitaryMatrix<5> expectedIdentity({{{1, 0, 0, 0, 0},
                                             {0, 1, 0, 0, 0},
                                             {0, 0, 1, 0, 0},
                                             {0, 0, 0, 1, 0},
                                             {0, 0, 0, 0, 1}}});

    EXPECT_EQ(DenseUnitaryMatrix<5>::identity(), expectedIdentity);
}

TEST(dense_unitary_matrix_test, reject_non_unitary_matrices) {
    DenseUnitaryMatrix<2>::Matrix nonUnitary1({{{1, 0}, {0, 1.1}}});

    EXPECT_THAT([&nonUnitary1]() { DenseUnitaryMatrix<2> test(nonUnitary1); },
        ::testing::ThrowsMessage<std::runtime_error>("Matrix is not unitary"));

    DenseUnitaryMatrix<2>::Matrix nonUnitary2({{{0, 0}, {0, 1}}});

    EXPECT_THAT([&nonUnitary2]() { DenseUnitaryMatrix<2> test(nonUnitary2); },
        ::testing::ThrowsMessage<std::runtime_error>("Matrix is not unitary"));

    DenseUnitaryMatrix<3>::Matrix nonUnitary3({{{1i, 0, 0}, {0, 1i, 1i}, {0, 1, 0}}});

    EXPECT_THAT([&nonUnitary3]() { DenseUnitaryMatrix<3> test(nonUnitary3); },
        ::testing::ThrowsMessage<std::runtime_error>("Matrix is not unitary"));
}

TEST(dense_unitary_matrix_test, dagger) {
    DenseUnitaryMatrix<2> m({{{1 / std::sqrt(2), 1 / std::sqrt(2)},
                              {1i / std::sqrt(2), -1i / std::sqrt(2)}}});

    DenseUnitaryMatrix<2> mDag({{{1 / std::sqrt(2), -1i / std::sqrt(2)},
                                 {1 / std::sqrt(2), 1i / std::sqrt(2)}}});

    EXPECT_EQ(m.dagger(), mDag);
}

}  // namespace qx::core
