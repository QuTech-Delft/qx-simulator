#include "qx/Core.hpp"

#include <gmock/gmock.h>  // ThrowsMessageHasSubstr
#include <gtest/gtest.h>
#include <stdexcept>  // runtime_error


namespace qx::core {

using namespace std::complex_literals;

TEST(sparse_array, set) {
    SparseArray victim(5);

    EXPECT_EQ(victim.testToVector(),
        std::vector<std::complex<double>>({0, 0, 0, 0, 0}));

    BasisVector key;
    key.set(2);
    victim.set(key, 1i);

    EXPECT_EQ(victim.testToVector(),
        std::vector<std::complex<double>>({0, 0, 0, 0, 1i}));

#ifndef NDEBUG
    key.set(1);
    ASSERT_EQ(key.toSizeT(), 6);
    EXPECT_THAT(([&victim, &key]() { victim.set(key, 0.1); }),
        ::testing::ThrowsMessage<std::runtime_error>("SparseArray::set index out of bounds"));
#endif
}

}  // namespace qx::core
