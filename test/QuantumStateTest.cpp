#include "qx/Core.hpp"
#include "qx/Gates.hpp"

#include <algorithm>  // count_if
#include <gtest/gtest.h>


namespace qx ::core {

using namespace std::complex_literals;

class QuantumStateTest : public ::testing::Test {
public:
    static void checkEq(QuantumState &victim,
                        std::vector<std::complex<double>> expected) {
        ASSERT_EQ(expected.size(), 1 << victim.getNumberOfQubits());

        std::size_t nonZeros = std::count_if(expected.begin(), expected.end(), isNotNull);

        victim.forEach([&nonZeros, &expected](auto const &kv) {
            EXPECT_GT(nonZeros, 0);
            EXPECT_NEAR(expected[kv.first.toSizeT()].real(), kv.second.real(),
                        .00000000000001);
            EXPECT_NEAR(expected[kv.first.toSizeT()].imag(), kv.second.imag(),
                        .00000000000001);
            --nonZeros;
        });
        EXPECT_EQ(nonZeros, 0);
    }
};

TEST_F(QuantumStateTest, apply_identity) {
    QuantumState victim(3);

    EXPECT_EQ(victim.getNumberOfQubits(), 3);
    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});

    victim.apply<3>(DenseUnitaryMatrix<8>::identity(),
                    std::array<QubitIndex, 3>{QubitIndex{0}, QubitIndex{1}, QubitIndex{2}});

    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});
}

TEST_F(QuantumStateTest, apply_hadamard) {
    QuantumState victim(3);

    EXPECT_EQ(victim.getNumberOfQubits(), 3);
    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});

    victim.apply<1>(
        DenseUnitaryMatrix<2>({{{1 / std::sqrt(2), 1 / std::sqrt(2)},
                                {1 / std::sqrt(2), -1 / std::sqrt(2)}}}),
        std::array<QubitIndex, 1>{QubitIndex{1}});

    checkEq(victim, {1 / std::sqrt(2), 0, 1 / std::sqrt(2), 0, 0, 0, 0, 0});
}

TEST_F(QuantumStateTest, apply_cnot) {
    QuantumState victim(2);
    victim.testInitialize({{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});

    victim.apply<2>(gates::CNOT, std::array<QubitIndex, 2>{QubitIndex{1}, QubitIndex{0}});

    checkEq(victim, {0, 0, std::sqrt(1 - std::pow(0.123, 2)), 0.123});
}

TEST_F(QuantumStateTest, measure_on_non_superposed_state) {
    QuantumState victim(2);
    victim.testInitialize({{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    victim.measure(QubitIndex{1}, []() { return 0.9485; });
    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});
    victim.measure(QubitIndex{1}, []() { return 0.045621; });
    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});

    EXPECT_EQ(victim.getMeasurementRegister(), BasisVector("10"));
}

TEST_F(QuantumStateTest, measure_on_superposed_state__case_0) {
    QuantumState victim(2);
    victim.testInitialize({{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    victim.measure(QubitIndex{0}, []() { return 0.994; });
    checkEq(victim, {0, 0, 1, 0});

    EXPECT_EQ(victim.getMeasurementRegister(), BasisVector("00"));
}

TEST_F(QuantumStateTest, measure_on_superposed_state__case_1) {
    QuantumState victim(2);
    victim.testInitialize({{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    victim.measure(QubitIndex{0}, []() { return 0.254; });
    checkEq(victim, {0, 0, 0, 1});

    EXPECT_EQ(victim.getMeasurementRegister(), BasisVector("01"));
}

TEST_F(QuantumStateTest, measure_all) {
    // measureAll cannot really be tested with superposition since it currently
    // relies on the iteration order of flat_hash_map which is unspecified.
    QuantumState victim(3);
    victim.testInitialize({{"110", 1i}});

    victim.measureAll([]() { return 0.994; });
    checkEq(victim, {0, 0, 0, 0, 0, 0, 1i, 0});

    EXPECT_EQ(victim.getMeasurementRegister(), BasisVector("110"));
}

TEST_F(QuantumStateTest, prep__case_0) {
    // Prep leads to a non-deterministic global quantum state because of the state collapse.
    QuantumState victim(2);
    victim.testInitialize({{"00", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    victim.prep(QubitIndex{0}, []() { return 0.994; });
    checkEq(victim, {1, 0, 0, 0});
}

TEST_F(QuantumStateTest, prep__case_1) {
    // Prep leads to a non-deterministic global quantum state because of the state collapse.
    QuantumState victim(2);
    victim.testInitialize({{"00", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    victim.prep(QubitIndex{0}, []() { return 0.245; });
    checkEq(victim, {0, 0, 1, 0});
}

} // namespace qx::core
