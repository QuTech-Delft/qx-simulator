#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/Core.h"
#include "doctest/doctest.h"

namespace qx {
namespace core {

using namespace std::complex_literals;

class DenseUnitaryMatrixTest {

};

TEST_CASE_FIXTURE(DenseUnitaryMatrixTest, "Identity") {
    DenseUnitaryMatrix<5> expectedIdentity({{{1, 0, 0, 0, 0},
                                            {0, 1, 0, 0, 0},
                                            {0, 0, 1, 0, 0},
                                            {0, 0, 0, 1, 0},
                                            {0, 0, 0, 0, 1}}});

    CHECK_EQ(DenseUnitaryMatrix<5>::identity(), expectedIdentity);
}

TEST_CASE_FIXTURE(DenseUnitaryMatrixTest, "Reject non-unitary matrices") {
    DenseUnitaryMatrix<2>::Matrix nonUnitary1({{{1, 0},
                                               {0, 1.1}}});

    CHECK_THROWS_WITH([&nonUnitary1] { DenseUnitaryMatrix<2> test(nonUnitary1); } (), "Matrix is not unitary");

    DenseUnitaryMatrix<2>::Matrix nonUnitary2({{{0, 0},
                                               {0, 1}}});

    CHECK_THROWS_WITH([&nonUnitary2] { DenseUnitaryMatrix<2> test(nonUnitary2); } (), "Matrix is not unitary");

    DenseUnitaryMatrix<3>::Matrix nonUnitary3({{{1i, 0, 0},
                                               {0, 1i, 1i},
                                               {0, 1, 0}}});

    CHECK_THROWS_WITH([&nonUnitary3] { DenseUnitaryMatrix<3> test(nonUnitary3); } (), "Matrix is not unitary");
}

TEST_CASE_FIXTURE(DenseUnitaryMatrixTest, "Dagger") {
    DenseUnitaryMatrix<2> m({{{1/std::sqrt(2), 1/std::sqrt(2)     },
                            {1i/std::sqrt(2) , -1i/std::sqrt(2)}}});

    DenseUnitaryMatrix<2> mDag({{{1/std::sqrt(2), -1i/std::sqrt(2)},
                                {1/std::sqrt(2), 1i/std::sqrt(2)}}});

    CHECK_EQ(m.dagger(), mDag);
}

}
}