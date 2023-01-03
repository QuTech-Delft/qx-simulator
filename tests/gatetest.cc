#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/core/gate.h"
#include "doctest/doctest.h"
#include <unordered_map>
#include <cmath>

namespace qx {

static const std::size_t N_QUBITS = 5;

class GateTest {
protected:
    void prepare_state(std::unordered_map<std::string, complex_t> state) {
        cvector_t v(1ULL << N_QUBITS, 0.0);

        double norm = 0;
        for (const auto& kv: state) {
            v[get_state_index(kv.first)] = kv.second;
            norm += kv.second.norm();
        }

        REQUIRE_EQ(norm, doctest::Approx(1));

        reg.set_data(v);
    }

    void apply(gate&& g) {
        g.apply(reg);
    }

    // For debug.
    void log_reg() {
        std::cout << reg.quantum_state() << std::endl;
    }

    void check_norm(std::string state_string, double expected_proba) {
        CAPTURE(state_string);

        CHECK_EQ(expected_proba, doctest::Approx(reg[get_state_index(state_string)].norm()));
    }

    void check_state(std::string state_string, complex_t expected_c) {
        CAPTURE(state_string);

        CHECK_EQ(expected_c.re, doctest::Approx(reg[get_state_index(state_string)].re));
        CHECK_EQ(expected_c.im, doctest::Approx(reg[get_state_index(state_string)].im));
    }

    void check_measurement_register(std::string expected) {
        CAPTURE(expected);

        REQUIRE_EQ(expected.size(), N_QUBITS);

        CHECK_EQ(reg.get_measurement_register(), std::bitset<MAX_QB_N>(expected));
    }

    // Three complex numbers c1, c2 and c3 such that |c1|^2+|c2|^2+|c3|^2 == 1.
    double m1 = 0.2;
    double m2 = 0.14;
    double m3 = sqrt(1 - pow(m1, 2) - pow(m2, 2));
    complex_t c1{m1*cos(1.2), m1*sin(1.2)};
    complex_t c2{m2*cos(2.3), m2*sin(2.3)};
    complex_t c3{m3*cos(3.4), m3*sin(3.4)};

private:
    static std::uint64_t get_state_index(std::string state_string) {
        REQUIRE_EQ(state_string.size(), N_QUBITS);

        std::bitset<MAX_QB_N> state(state_string);

        return state.to_ulong();
    }

    qu_register reg{N_QUBITS};
};

TEST_CASE_FIXTURE(GateTest, "Hadamard") {
    apply(hadamard(0));

    check_norm("00000", 0.5);
    check_norm("00001", 0.5);
}

TEST_CASE_FIXTURE(GateTest, "Bell pair") {
    apply(hadamard(3));
    apply(cnot(3,4));

    check_norm("00000", 0.5);
    check_norm("11000", 0.5);
}

TEST_CASE_FIXTURE(GateTest, "Pauli X") {
    apply(pauli_x(2));

    check_norm("00100", 1);
}

TEST_CASE_FIXTURE(GateTest, "Swap") {
    SUBCASE("Simple state") {
        prepare_state({
            {"00010", complex_t(0, 0)},
            {"10000", complex_t(0, 1)}
        });

        apply(swap(1, 4));

        check_state("00010", complex_t(0, 1));
        check_state("10000", complex_t(0, 0));
    }
    
    SUBCASE("Entangled states") {
        prepare_state({
            {"00110", c1},
            {"10001", c2},
            {"10010", c3}
        });

        apply(swap(1, 3));

        log_reg();

        check_state("01100", c1);
        check_state("10001", c2);
        check_state("11000", c3);
    }
}

TEST_CASE_FIXTURE(GateTest, "Measure all zeros") {
    apply(measure(0, [](qu_register&) { return 0.123456789; }));
    check_measurement_register("00000");

    check_state("00000", complex_t(1, 0));
}

TEST_CASE_FIXTURE(GateTest, "Measure after hadamard on qubit 3 returns 1") {
    apply(hadamard(3));
    apply(measure(3, [](qu_register&) { return 0.45; }));
    check_measurement_register("01000");

    check_state("01000", complex_t(1, 0));
}

TEST_CASE_FIXTURE(GateTest, "Measure after hadamard on qubit 3 returns 0") {
    apply(hadamard(3));
    apply(measure(3, [](qu_register&) { return 0.7; }));
    check_measurement_register("00000");

    check_state("00000", complex_t(1, 0));
}

TEST_CASE_FIXTURE(GateTest, "Measure qubit 1") {
    auto rand = 0.;
    prepare_state({
        {"00110", c1},
        {"10001", c2},
        {"10010", c3}
    });

    SUBCASE("Returns 1") {
        SUBCASE("A") {
            rand = 0.001;
        }
        SUBCASE("B") {
            rand = pow(m1, 2) + pow(m3, 2) - 0.0000854;
        }

        apply(measure(1, [rand](qu_register&) { return rand; }));
        check_measurement_register("00010");

        check_state("00110", c1 / sqrt(pow(m1, 2) + pow(m3, 2)));
        check_state("10010", c3 / sqrt(pow(m1, 2) + pow(m3, 2)));
    }

    SUBCASE("Returns 0") {
        SUBCASE("A") {
            rand = pow(m1, 2) + pow(m3, 2) + 0.0000198;
        }
        SUBCASE("B") {
            rand = 0.99;
        }

        apply(measure(1, [rand](qu_register&) { return rand; }));
        check_measurement_register("00000");

        check_state("10001", c2 / sqrt(pow(m2, 2)));
    }
}

TEST_CASE_FIXTURE(GateTest, "Measure all qubits") {
    auto rand = 0.;
    prepare_state({
        {"00110", c1},
        {"10001", c2},
        {"10010", c3}
    });

    SUBCASE("00110") {
        SUBCASE("A") {
            rand = 0.001;
        }
        SUBCASE("B") {
            rand = pow(m1, 2) - 0.000026;
        }

        apply(measure_all([rand](qu_register&) { return rand; }));
        check_measurement_register("00110");

        check_state("00110", c1 / sqrt(pow(m1, 2)));
    }

    SUBCASE("10001") {
        SUBCASE("A") {
            rand = pow(m1, 2) + 0.00001;
        }
        SUBCASE("B") {
            rand = pow(m1, 2) + pow(m2, 2) - 0.0002;
        }

        apply(measure_all([rand](qu_register&) { return rand; }));
        check_measurement_register("10001");

        check_state("10001", c2 / sqrt(pow(m2, 2)));
    }

    SUBCASE("10010") {
        SUBCASE("A") {
            rand = pow(m1, 2) + pow(m2, 2) + 0.0000005;
        }
        SUBCASE("B") {
            rand = 0.99;
        }

        apply(measure_all([rand](qu_register&) { return rand; }));
        check_measurement_register("10010");

        check_state("10010", c3 / sqrt(pow(m3, 2)));
        check_norm("10001", 0.);
        check_norm("00110", 0.);
    }
}

// TODO: add test cases for other gates.

}