#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/Core.h"
#include "doctest/doctest.h"

namespace qx {
namespace core {

using namespace std::complex_literals;

class SparseArrayTest {

};

TEST_CASE_FIXTURE(SparseArrayTest, "Set") {
    SparseArray victim(5);

    CHECK_EQ(victim.testToVector(), std::vector<std::complex<double>>({0, 0, 0, 0, 0}));

    BasisVector key;
    key.set(2);
    victim.set(key, 1i);

    CHECK_EQ(victim.testToVector(), std::vector<std::complex<double>>({0, 0, 0, 0, 1i}));
    
#ifndef NDEBUG
    key.set(1);
    REQUIRE_EQ(key.toSizeT(), 6);
    CHECK_THROWS_WITH(victim.set(key, 0.1), "SparseArray::set index out of bounds");
#endif
}

}
}