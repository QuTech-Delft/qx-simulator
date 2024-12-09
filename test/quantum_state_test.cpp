#include "qx/core.hpp" // BasisVector
#include "qx/gates.hpp"
#include "qx/quantum_state.hpp"

#include <algorithm>  // count_if
#include <gtest/gtest.h>


namespace qx ::core {

using namespace std::complex_literals;

class QuantumStateTest : public ::testing::Test {
public:
    static void checkEq(QuantumState &victim,
                        std::vector<std::complex<double>> expected) {
        ASSERT_EQ(expected.size(), 1 << victim.get_number_of_qubits());

        std::size_t non_zeros = std::count_if(expected.begin(), expected.end(), is_not_null);

        victim.for_each([&non_zeros, &expected](auto const &sparseElement) {
            auto const &[basis_vector, sparse_complex] = sparseElement;
            EXPECT_GT(non_zeros, 0);
            EXPECT_NEAR(expected[basis_vector.to_size_t()].real(), sparse_complex.value.real(),
                        .000'000'000'000'01);
            EXPECT_NEAR(expected[basis_vector.to_size_t()].imag(), sparse_complex.value.imag(),
                        .000'000'000'000'01);
            --non_zeros;
        });
        EXPECT_EQ(non_zeros, 0);
    }
};

TEST_F(QuantumStateTest, apply_identity) {
    QuantumState victim{ 3, 3 };
    EXPECT_EQ(victim.get_number_of_qubits(), 3);
    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});
    victim.apply<3>(DenseUnitaryMatrix<8>::identity(),
                    std::array<QubitIndex, 3>{QubitIndex{0}, QubitIndex{1}, QubitIndex{2}});
    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});
}

TEST_F(QuantumStateTest, apply_hadamard) {
    QuantumState victim{ 3, 3 };
    EXPECT_EQ(victim.get_number_of_qubits(), 3);
    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});
    victim.apply<1>(gates::H, std::array<QubitIndex, 1>{QubitIndex{1}});
    checkEq(victim, {1 / std::sqrt(2), 0, 1 / std::sqrt(2), 0, 0, 0, 0, 0});
}

TEST_F(QuantumStateTest, apply_cnot) {
    QuantumState victim{ 2, 2, {{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}} };
    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});
    victim.apply<2>(gates::CNOT, std::array<QubitIndex, 2>{QubitIndex{1}, QubitIndex{0}});
    checkEq(victim, {0, 0, std::sqrt(1 - std::pow(0.123, 2)), 0.123});
}

TEST_F(QuantumStateTest, measure_on_non_superposed_state) {
    auto measurement_register = core::BasisVector{};
    auto bit_measurement_register = core::BitMeasurementRegister{ 2 };
    QuantumState victim{ 2, 2, {{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}} };
    victim.apply_measure(
        QubitIndex{ 1 }, BitIndex{ 1 }, []() { return 0.9485; }, measurement_register, bit_measurement_register);
    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});
    victim.apply_measure(
        QubitIndex{ 1 }, BitIndex{ 0 }, []() { return 0.045621; }, measurement_register, bit_measurement_register);
    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});
    EXPECT_EQ(measurement_register, BasisVector{ "10" });
}

TEST_F(QuantumStateTest, measure_on_superposed_state__measured_state_is_0) {
    // The random generator function returns a number bigger than the probability of measuring 1, so we measure 0
    // 0.994 > 1 - 0.123^2
    auto measurement_register = core::BasisVector{};
    auto bit_measurement_register = core::BitMeasurementRegister{ 2 };
    QuantumState victim{ 2, 2, {{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}} };
    victim.apply_measure(
        QubitIndex{ 0 }, BitIndex{ 0 }, []() { return 0.994; }, measurement_register, bit_measurement_register);
    checkEq(victim, {0, 0, 1, 0});  // 10
    EXPECT_EQ(measurement_register, BasisVector{ "00" });
}

TEST_F(QuantumStateTest, measure_on_superposed_state__measured_state_is_1) {
    // The random generator function returns a number smaller than the probability of measuring 1, so we measure 1
    // 0.254 < 1 - 0.123^2
    auto measurement_register = core::BasisVector{};
    auto bit_measurement_register = core::BitMeasurementRegister{ 2 };
    QuantumState victim{ 2, 2, {{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}} };
    victim.apply_measure(
        QubitIndex{ 0 }, BitIndex{ 0 }, []() { return 0.254; }, measurement_register, bit_measurement_register);
    checkEq(victim, {0, 0, 0, 1});  // 11
    EXPECT_EQ(measurement_register, BasisVector{ "01" });
}

TEST_F(QuantumStateTest, reset) {
    QuantumState victim{ 2, 2, {{"00", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}} };
    victim.apply_reset(QubitIndex{ 0 });
    checkEq(victim, {0.123, 0, std::sqrt(1 - std::pow(0.123, 2)), 0});  // 00 and 10
}

TEST_F(QuantumStateTest, reset_all) {
    QuantumState victim{ 2, 2, {{"00", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}} };
    victim.apply_reset_all();
    checkEq(victim, QuantumState{ 2, 2 }.to_vector());
}

} // namespace qx::core
