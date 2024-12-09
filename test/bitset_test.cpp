#include "qx/utils.hpp"

#include <gtest/gtest.h>


namespace qx::utils {

TEST(Bitset, set_test) {
    Bitset<15> victim{};
    EXPECT_FALSE(victim.test(1));
    EXPECT_FALSE(victim.test(13));

    EXPECT_EQ(victim.to_size_t(), 0);
    victim.set(0);
    EXPECT_FALSE(victim.test(1));
    EXPECT_EQ(victim.to_size_t(), 1);
    victim.set(1);
    EXPECT_EQ(victim.to_size_t(), 3);
    EXPECT_TRUE(victim.test(1));
    EXPECT_TRUE(victim.test(0));
    EXPECT_FALSE(victim.test(10));

    victim.set(3);
    EXPECT_TRUE(victim.test(3));
    EXPECT_EQ(victim.to_size_t(), 11);
}

TEST(Bitset, set_test_64) {
    Bitset<64> victim{};

    victim.set(31);

    EXPECT_TRUE(victim.test(31));
    for (auto i = 0; i < 64; ++i) {
        if (i != 31) {
            EXPECT_FALSE(victim.test(i));
        }
    }
}

TEST(Bitset, set_test_with_a_lot_of_bits) {
    Bitset<150> victim{};
    EXPECT_FALSE(victim.test(120));
    EXPECT_FALSE(victim.test(130));

    victim.set(0);
    EXPECT_TRUE(victim.test(0));
    EXPECT_FALSE(victim.test(1));
    victim.set(132);
    EXPECT_TRUE(victim.test(0));
    EXPECT_TRUE(victim.test(132));
    EXPECT_FALSE(victim.test(149));

    EXPECT_EQ(victim.to_string(),
              "00000000000000000100000000000000000000000000000000000000000000000"
              "00000000000000000000000000000000000000000000000000000000000000000"
              "00000000000000000001");
}

TEST(Bitset, from_string) {
    Bitset<5> victim{"1010"};
    EXPECT_FALSE(victim.test(0));
    EXPECT_TRUE(victim.test(1));
    EXPECT_FALSE(victim.test(2));
    EXPECT_TRUE(victim.test(3));
    EXPECT_FALSE(victim.test(4));
    EXPECT_EQ(victim.to_size_t(), 10);
}

TEST(Bitset, to_string) {
    Bitset<15> victim{};
    victim.set(0);
    EXPECT_EQ(victim.to_string(), "000000000000001");

    victim.set(13);
    EXPECT_EQ(victim.to_string(), "010000000000001");
}

// Requires GMock
// TEST(Bitset, hash) {
//     Bitset<15> victim1{};
//     EXPECT_TRUE(absl::VerifyTypeImplementsAbslHashCorrectly({victim1}));

//     Bitset<15456> victim2{};
//     victim2.set(4542);
//     victim2.set(8945);
//     EXPECT_TRUE(absl::VerifyTypeImplementsAbslHashCorrectly({victim2}));
// }

TEST(Bitset, operator_xor__small_bitset) {
    Bitset<15> victim{"000010000010001"};
    Bitset<15> mask{"000011001000001"};

    victim ^= mask;
    EXPECT_EQ(victim.to_string(), "000001001010000");
}

TEST(Bitset, operator_xor__large_bitset) {
    Bitset<123456> victim{};
    victim.set(457);

    Bitset<123456> mask{};

    mask.set(457);
    mask.set(654);

    victim ^= mask;

    EXPECT_FALSE(victim.test(457));
    EXPECT_TRUE(victim.test(654));
    EXPECT_FALSE(victim.test(875));
}

} // namespace qx::utils
