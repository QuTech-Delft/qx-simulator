#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/Utils.h"
#include "doctest/doctest.h"
// #include "absl/hash/hash_testing.h"

namespace qx {
namespace utils {

class BitsetTest {
public:
};

TEST_CASE_FIXTURE(BitsetTest, "Set/test") {
    Bitset<15> victim{};
    CHECK(!victim.test(1));
    CHECK(!victim.test(13));

    CHECK_EQ(victim.toSizeT(), 0);
    victim.set(0);
    CHECK(!victim.test(1));
    CHECK_EQ(victim.toSizeT(), 1);
    victim.set(1);
    CHECK_EQ(victim.toSizeT(), 3);
    CHECK(victim.test(1));
    CHECK(victim.test(0));
    CHECK(!victim.test(10));

    victim.set(3);
    CHECK(victim.test(3));
    CHECK_EQ(victim.toSizeT(), 11);
}

TEST_CASE_FIXTURE(BitsetTest, "Set/test with a lot of bits") {
    Bitset<150> victim{};
    CHECK(!victim.test(120));
    CHECK(!victim.test(130));

    victim.set(0);
    CHECK(victim.test(0));
    CHECK(!victim.test(1));
    victim.set(132);
    CHECK(victim.test(0));
    CHECK(victim.test(132));
    CHECK(!victim.test(149));
    
    CHECK_EQ(victim.toString(), "000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
}

TEST_CASE_FIXTURE(BitsetTest, "From string") {
    Bitset<5> victim{"1010"};
    CHECK(!victim.test(0));
    CHECK(victim.test(1));
    CHECK(!victim.test(2));
    CHECK(victim.test(3));
    CHECK(!victim.test(4));
    CHECK_EQ(victim.toSizeT(), 10);
}

TEST_CASE_FIXTURE(BitsetTest, "toString") {
    Bitset<15> victim{};
    victim.set(0);
    CHECK_EQ(victim.toString(), "000000000000001");

    victim.set(13);
    CHECK_EQ(victim.toString(), "010000000000001");
}

// Requires GMock
// TEST_CASE_FIXTURE(BitsetTest, "Hash") {
//     Bitset<15> victim1{};
//     CHECK(absl::VerifyTypeImplementsAbslHashCorrectly({victim1}));

//     Bitset<15456> victim2{};
//     victim2.set(4542);
//     victim2.set(8945);
//     CHECK(absl::VerifyTypeImplementsAbslHashCorrectly({victim2}));
// }

}
}