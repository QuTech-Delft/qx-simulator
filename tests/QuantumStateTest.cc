#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/Core.h"
#include "qx/Gates.h"
#include "doctest/doctest.h"

namespace qx {
namespace core {

using namespace std::complex_literals;

class QuantumStateTest {
public:
    static void checkEq(QuantumState& victim, std::vector<std::complex<double>> expected) {
        REQUIRE_EQ(expected.size(), 1 << victim.getNumberOfQubits());

        std::size_t nonZeros = 0;
        for (auto c: expected) {
            if (isNotNull(c)) {
                ++nonZeros;
            }
        }

        victim.forEach([&nonZeros, &expected](auto const& kv) {
            CHECK_GT(nonZeros, 0);
            CHECK(expected[kv.first.toSizeT()].real() == doctest::Approx(kv.second.real()));
            CHECK(expected[kv.first.toSizeT()].imag() == doctest::Approx(kv.second.imag()));
            --nonZeros;
        });
        CHECK_EQ(nonZeros, 0);
    }
};

TEST_CASE_FIXTURE(QuantumStateTest, "Apply identity") {
    QuantumState victim(3);

    CHECK_EQ(victim.getNumberOfQubits(), 3);
    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});

    victim.apply<3>(DenseUnitaryMatrix<8>::identity(), {0, 1, 2});

    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});
}

TEST_CASE_FIXTURE(QuantumStateTest, "Apply hadamard") {
    QuantumState victim(3);

    CHECK_EQ(victim.getNumberOfQubits(), 3);
    checkEq(victim, {1, 0, 0, 0, 0, 0, 0, 0});

    victim.apply<1>(DenseUnitaryMatrix<2>({{{1/std::sqrt(2), 1/std::sqrt(2)}, {1/std::sqrt(2), -1/std::sqrt(2)}}}), {1});

    checkEq(victim, {1/std::sqrt(2), 0, 1/std::sqrt(2), 0, 0, 0, 0, 0});
}

TEST_CASE_FIXTURE(QuantumStateTest, "Apply cnot") {
    QuantumState victim(2);
    victim.testInitialize({{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});

    victim.apply<2>(gates::CNOT, {1, 0});

    checkEq(victim, {0, 0, std::sqrt(1 - std::pow(0.123, 2)), 0.123});
}

TEST_CASE_FIXTURE(QuantumStateTest, "Measure on non-superposed state") {
    QuantumState victim(2);
    victim.testInitialize({{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    victim.measure(QubitIndex{1}, []() { return 0.9485; });
    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});
    victim.measure(QubitIndex{1}, []() { return 0.045621; });
    checkEq(victim, {0, 0, 0.123, std::sqrt(1 - std::pow(0.123, 2))});

    CHECK_EQ(victim.getMeasurementRegister(), BasisVector("10"));
}

TEST_CASE_FIXTURE(QuantumStateTest, "Measure on superposed state") {
    QuantumState victim(2);
    victim.testInitialize({{"10", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    SUBCASE("Measure 0") {
        victim.measure(QubitIndex{0}, []() { return 0.994; });
        checkEq(victim, {0, 0, 1, 0});

        CHECK_EQ(victim.getMeasurementRegister(), BasisVector("00"));
    }

    SUBCASE("Measure 1") {
        victim.measure(QubitIndex{0}, []() { return 0.254; });
        checkEq(victim, {0, 0, 0, 1});

        CHECK_EQ(victim.getMeasurementRegister(), BasisVector("01"));
    }
}

TEST_CASE_FIXTURE(QuantumStateTest, "Measure all") {
    QuantumState victim(3);

    // measureAll cannot really be tested with superposition since it currently relies on the iteration order of flat_hash_map
    // which is unspecified.
    victim.testInitialize({{"110", 1i}});

    victim.measureAll([]() { return 0.994; });
    checkEq(victim, {0, 0, 0, 0, 0, 0, 1i, 0});

    CHECK_EQ(victim.getMeasurementRegister(), BasisVector("110"));
}

TEST_CASE_FIXTURE(QuantumStateTest, "Prep") {
    // Prep leads to a non-deterministic global quantum state because of the state collapse.

    QuantumState victim(2);
    victim.testInitialize({{"00", 0.123}, {"11", std::sqrt(1 - std::pow(0.123, 2))}});

    SUBCASE("Case 0") {
        victim.prep(QubitIndex{0}, []() { return 0.994; });
        checkEq(victim, {1, 0, 0, 0});
    }

    SUBCASE("Case 1") {
        victim.prep(QubitIndex{0}, []() { return 0.245; });
        checkEq(victim, {0, 0, 1, 0});
    }
}

}
}