#include "qx/SparseArray.hpp"

#include <complex>
#include <gmock/gmock.h>  // ThrowsMessageHasSubstr
#include <gtest/gtest.h>
#include <vector>


namespace qx::core {

using namespace std::complex_literals;

TEST(sparse_array, set) {
    SparseArray victim{ 5 };
    EXPECT_EQ(victim.toVector(), (std::vector<std::complex<double>>{ 0., 0., 0., 0., 0. }));

    BasisVector key;
    key.set(2);
    victim[key] = SparseComplex{ 1i };
    EXPECT_EQ(victim.toVector(), (std::vector<std::complex<double>>{ 0., 0., 0., 0., 1i }));

#ifndef NDEBUG
    key.set(1);
    ASSERT_EQ(key.toSizeT(), 6);
    EXPECT_THAT(([&victim, &key]() { victim[key] = SparseComplex{ 0.1 }; }),
        ::testing::ThrowsMessage<SparseArrayError>("index out of bounds"));
#endif
}

}  // namespace qx::core
