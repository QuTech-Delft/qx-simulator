#include <gmock/gmock.h>  // ThrowsMessage
#include <gtest/gtest.h>

#include <cmath>  // abs
#include <optional>  // nullopt
#include <stdexcept>  // runtime_error

#include "qx/simulator.hpp"

namespace qx {

class IntegrationTest : public ::testing::Test {
public:
    static SimulationResult run_from_string(
        const std::string& s, std::uint64_t iterations = 1, std::string cqasm_version = "3.0") {
        auto result = execute_string(s, iterations, std::nullopt, std::move(cqasm_version));
        EXPECT_TRUE(std::holds_alternative<SimulationResult>(result));
        return *std::get_if<SimulationResult>(&result);
    }
};

TEST_F(IntegrationTest, bell_pair) {
    auto program = R"(
version 3.0

qubit[2] q

H q[0]
CNOT q[0], q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    EXPECT_EQ(actual.shots_requested, iterations);
    EXPECT_EQ(actual.shots_done, iterations);

    // Expected 'q' state should be |00>+|11>
    // State is |00>+|11> after creating the Bell state
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "11", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, range_operands) {
    auto program = R"(
version 3.0

qubit[6] q
X q[0:2]
CNOT q[0:2], q[3:5]
)";
    std::size_t iterations = 2;
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |111111>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "111111", core::Complex{ .real = 1, .imag = 0, .norm = 1 } }
    }));
}

TEST_F(IntegrationTest, too_many_qubits) {
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(execute_string("version 3.0; qubit[62] q")));
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(execute_string("version 3.0; qubit[63] q")));
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(execute_string("version 3.0; qubit[64] q")));

    EXPECT_TRUE(std::holds_alternative<SimulationError>(execute_string("version 3.0; qubit[65] q")));
    EXPECT_TRUE(std::holds_alternative<SimulationError>(execute_string("version 3.0; qubit[66] q")));
}

TEST_F(IntegrationTest, syntax_error) {
    auto program = R"(
version 3.0

qubit q

H q[0
)";

    auto result = execute_string(program);
    auto message = std::string{ std::get_if<SimulationError>(&result)->what() };
    EXPECT_TRUE(std::holds_alternative<SimulationError>(result));
    EXPECT_THAT(message, ::testing::StartsWith(R"(cQASM v3 analyzer returned errors:
Error at <unknown file name>:6:6..7: missing ']' at '\n')"));
}

TEST_F(IntegrationTest, identity) {
    auto program = R"(
version 3.0

qubit[2] q

H q[0]
I q[0]
CNOT q[0], q[1]
I q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    EXPECT_EQ(actual.shots_requested, iterations);
    EXPECT_EQ(actual.shots_done, iterations);

    // Expected 'q' state should be |00>+|11>
    // State is |00>+|11> after creating the Bell state
    // Identity gates do not modify the state of the qubits
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "11", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, swap) {
    auto program = R"(
version 3.0

qubit[2] q

H q[0]
CZ q[0], q[1]
SWAP q[0], q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|10> after SWAP
    // State is |00>+|01> after H, then CZ does not change the state of qubit 1 because Z|0>=|0>,
    // and SWAP exchanges states for qubits 0 and 1
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "10", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, measure) {
    auto program = R"(
version 3.0

qubit[3] q
bit[3] b

X q[0]
H q[1]
CNOT q[1], q[2]

b = measure q
)";
    std::size_t iterations = 10'000;
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |001> or |111>
    EXPECT_TRUE(actual.state[0].value.ends_with('1'));
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected 'b' value should be "001" 50% of the cases and "111" 50% of the cases
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations) / 2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_EQ(actual.measurements[0].state, "001");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[0].count)), error);
    EXPECT_EQ(actual.measurements[1].state, "111");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[1].count)), error);
}

TEST_F(IntegrationTest, multiple_measure_instructions) {
    auto program = R"(
version 3.0

qubit[3] q
bit[3] b

X q[0]
H q[1]
CNOT q[1], q[2]

b[0] = measure q[0]
b[1] = measure q[1]
b[2] = measure q[2]
)";
    std::size_t iterations = 10'000;
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |001> or |111>
    EXPECT_TRUE(actual.state[0].value.ends_with('1'));
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected 'b' value should be "001" 50% of the cases and "111" 50% of the cases
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations) / 2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_EQ(actual.measurements[0].state, "001");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[0].count)), error);
    EXPECT_EQ(actual.measurements[1].state, "111");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[1].count)), error);
}

TEST_F(IntegrationTest, mid_circuit_measure_instruction) {
    auto program = R"(
version 3.0

qubit[2] q
bit[2] b

X q[0]
b[0] = measure q[0]

H q[1]
CNOT q[1], q[0]
b = measure q
)";
    std::size_t iterations = 10'000;
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |01>+|10>
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations) / 2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_TRUE(actual.measurements[0].state == "01");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[0].count)), error);
    EXPECT_TRUE(actual.measurements[1].state == "10");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[1].count)), error);
}

TEST_F(IntegrationTest, multiple_qubit_bit_definitions_and_mid_circuit_measure_instructions) {
    auto program = R"(
version 3.0

qubit q0
X q0
bit b0
b0 = measure q0

qubit q1
H q1
CNOT q1, q0
bit b1
b0 = measure q0
b1 = measure q1
)";
    std::size_t iterations = 10'000;
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |01>+|10>
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations) / 2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_TRUE(actual.measurements[0].state == "01");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[0].count)), error);
    EXPECT_TRUE(actual.measurements[1].state == "10");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[1].count)), error);
}

TEST_F(IntegrationTest, reset__x_measure_reset) {
    auto program = R"(
version 3.0

qubit q
bit b
X q
b = measure q
reset q
)";
    std::size_t iterations = 10'000;
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should always be |0> because 'reset' modifies the qubit state
    EXPECT_EQ(actual.state[0].value, "0");
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected 'b' value should always be '1' because 'reset' does not modify the measurement register
    EXPECT_EQ(actual.measurements.size(), 1);
    EXPECT_TRUE(actual.measurements[0].state == "1");
    EXPECT_EQ(actual.measurements[0].count, iterations);
}

TEST_F(IntegrationTest, reset__bell_state_then_reset) {
    std::size_t iterations = 10'000;
    auto program = R"(
version 3.0

qubit[2] q
bit[2] b
H q[0]
CNOT q[0], q[1]
reset q[0]
)";
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |00>+|10>
    // State is |00>+|11> after creating the Bell state
    // Then 'reset' sets q to |0>, leaving the state as |00>+|10>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "10", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, reset__bell_state_then_reset_and_measure) {
    std::size_t iterations = 10'000;
    auto program = R"(
version 3.0

qubit[2] q
bit[2] b
H q[0]
CNOT q[0], q[1]
reset q[0]
b = measure q
)";
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |00>+|10>
    // State is |00>+|11> after creating the Bell state
    // Then 'reset' sets q to |0>, leaving the state as |00>+|10>
    // 'measure' provokes a collapse of the state to either |00> or |10>
    EXPECT_TRUE(actual.state[0].value.ends_with('0'));
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected b value should be "00" 50% of the cases and "10" 50% of the cases
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations) / 2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_TRUE(actual.measurements[0].state == "00" || actual.measurements[0].state == "10");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[0].count)), error);
}

TEST_F(IntegrationTest, reset__bell_state_then_measure_and_reset) {
    std::size_t iterations = 10'000;
    auto program = R"(
version 3.0

qubit[2] q
bit[2] b
H q[0]
CNOT q[0], q[1]
b = measure q
reset q[0]
)";
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |00> or |10>
    // State is |00>+|11> after creating the Bell state
    // 'measure' provokes a collapse of the state to either |00> or |11>
    // Then 'reset' sets q0 to |0>, leaving the state as |00> or |10>
    EXPECT_TRUE(actual.state[0].value.ends_with('0'));
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected 'b' value should be '00' 50% of the cases and '11' 50% of the cases
    // because 'measure' happens right after creating the Bell state,
    // and 'reset' does not modify the measurement register
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations) / 2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_TRUE(actual.measurements[0].state == "00" || actual.measurements[0].state == "11");
    EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - actual.measurements[0].count)), error);
}

TEST_F(IntegrationTest, bit_measurement_register) {
    auto program = R"(
version 3.0

qubit[2] qq
X qq[0]
bit[2] bb
bb[0] = measure qq[0]

qubit q
H q
CNOT q, qq[0]
bit b
b = measure qq[0]
)";
    std::size_t iterations = 10'000;
    auto actual = run_from_string(program, iterations);

    // Expected bb[0] value should always be '1'
    // Expected 'b' value should be '0' 50% of the cases and '1' 50% of the cases
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations) / 2 * 0.05);
    EXPECT_EQ(actual.bit_measurements.size(), 2);
    for (const auto& bit_measurement : actual.bit_measurements) {
        EXPECT_EQ(actual.get_bit_measurement(bit_measurement.state, "bb", 0), 1);
        EXPECT_LT(std::abs(static_cast<long long>(iterations / 2 - bit_measurement.count)), error);
    }
}

TEST_F(IntegrationTest, correct_init_is_just_discarded_by_the_simulator) {
    auto program = R"(
version 3.0

qubit[2] q

init q[0]
H q[0]
barrier q[1]
init q[1]
CNOT q[0], q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|10>
    // State is |00>+|11> after creating the Bell state
    // A correct 'init' instruction is discarded by the simulator
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "11", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, incorrect_init_throws_error) {
    auto program = R"(
version 3.0

qubit[2] qq

H qq[0]
CNOT qq[0], qq[1]
init qq[0]
)";
    auto result = execute_string(program);
    auto message = std::string{ std::get_if<SimulationError>(&result)->what() };
    EXPECT_TRUE(std::holds_alternative<SimulationError>(result));
    EXPECT_EQ(message, "incorrect 'init qq[0]': the qubit has already been used in a non-control instruction");
}

TEST_F(IntegrationTest, barrier_and_correct_wait_are_just_discarded_by_the_simulator) {
    auto program = R"(
version 3.0

qubit[2] q

barrier q
wait(0) q[0]
H q[0]
barrier q[0]
wait(1) q[1]
CNOT q[0], q[1]
barrier q[1]
wait(2) q
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|11>
    // State is |00>+|11> after creating the Bell state
    // 'barrier' and 'wait' instructions are just discarded by the simulator
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "11", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, incorrect_wait_throws_error) {
    auto program = R"(
version 3.0

qubit[2] q

barrier q
wait(-1) q[0]
H q[0]
)";
    auto result = execute_string(program);
    auto message = std::string{ std::get_if<SimulationError>(&result)->what() };
    EXPECT_TRUE(std::holds_alternative<SimulationError>(result));
    EXPECT_EQ(message, "incorrect 'wait(-1) q[0]': time cannot be negative");
}

TEST_F(IntegrationTest, inverse_gate_modifier__inv_inv) {
    auto program = R"(
version 3.0

qubit q

inv.inv.H q
)";
    std::size_t iterations = 10'000;
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |0>+|1> as inv.inv.H is equivalent to H
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "0", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "1", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, power_gate_modifier__pow_2_s) {
    auto program = R"(
version 3.0

qubit q

pow(2).X q
)";
    std::size_t iterations = 10'000;
    auto actual = run_from_string(program, iterations);

    // Expected 'q' state should be |0> as pow(2).X is equivalent to I
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "0", core::Complex{ .real = 1, .imag = 0, .norm = 1 } }
    }));
}

TEST_F(IntegrationTest, control_gate_modifier__ctrl_x) {
    auto program = R"(
version 3.0

qubit[2] q

H q[0]
ctrl.X q[0], q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|11> as ctrl.X is equivalent to CNOT
    // State is |00>+|11> after creating the Bell state
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "11", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, control_gate_modifier__ctrl_x90) {
    auto program = R"(
version 3.0

qubit[2] q

H q[0]
ctrl.X90 q[0], q[1]
H q[0]
H q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|10>+|11>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00",                  core::Complex{ .real = 1. / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "10",  core::Complex{ .real = gates::SQRT_2 / 4, .imag = gates::SQRT_2 / 4, .norm = 0.25 } },
            { "11", core::Complex{ .real = gates::SQRT_2 / 4, .imag = -gates::SQRT_2 / 4, .norm = 0.25 } }
    }));
}

TEST_F(IntegrationTest, control_gate_modifier__ctrl_x90_after_h_q1) {
    auto program = R"(
version 3.0

qubit[2] q

H q[1]
ctrl.X90 q[0], q[1]
H q[0]
H q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|01>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1. / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "01", core::Complex{ .real = 1. / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, control_gate_modifier__ctrl_rx_half_pi) {
    auto program = R"(
version 3.0

qubit[2] q

H q[0]
ctrl.Rx(pi/2) q[0], q[1]
H q[0]
H q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|01>+|10>+|11>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00",
             core::Complex{ .real = (1. + gates::SQRT_2) / 4, .imag = -0.25, .norm = (2. + gates::SQRT_2) / 8 }      },
            { "01",
             core::Complex{ .real = (-1. + gates::SQRT_2) / 4, .imag = 0.25, .norm = (2. - gates::SQRT_2) / 8 }      },
            { "10", core::Complex{ .real = (1. + gates::SQRT_2) / 4, .imag = 0.25, .norm = (2. + gates::SQRT_2) / 8 } },
            { "11",
             core::Complex{
             .real = (-1. + gates::SQRT_2) / 4, .imag = -0.25, .norm = (2. - gates::SQRT_2) / 8 }                    }
    }));
}

TEST_F(IntegrationTest, gate_modifier__ctrl_pow_2_s) {
    auto program = R"(
version 3.0

qubit[2] q

H q[0]
ctrl.pow(2).S q[0], q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|01> as ctrl.pow(2).S is equivalent to CZ
    // State is |00>+|01> after H, then CZ does not change the state of qubit 1 because Z|0>=|0>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "01", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, gate_modifier__ctrl_pow_2_sqrt_s) {
    auto program = R"(
version 3.0

qubit[2] q

H q[0]
ctrl.pow(2).pow(1./2).Z q[0], q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |00>+|01> as ctrl.pow(2).pow(1./2).Z is equivalent to CZ
    // State is |00>+|01> after H, then CZ does not change the state of qubit 1 because Z|0>=|0>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "01", core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, gate_modifier__ctrl_pow_1_3_z) {
    auto program = R"(
version 3.0

qubit[2] q

X q[1]
H q[0]
ctrl.pow(1./3).Z q[0], q[1]
)";
    std::size_t iterations = 1;
    auto actual = run_from_string(program, iterations, "3.0");

    // Expected 'q' state should be |10>+|11>
    // State is |10>+|11> after H, then ctrl.pow(1./3).Z adds a phase shift
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "10",core::Complex{ .real = 1 / gates::SQRT_2, .imag = 0, .norm = 0.5 } },
            { "11",
             core::Complex{ .real = std::cos(gates::PI / 3) / gates::SQRT_2,
             .imag = std::sin(gates::PI / 3) / gates::SQRT_2,
             .norm = 0.5 }                                         }
    }));
}

TEST_F(IntegrationTest, power_gate_modifier__fractional_exponent) {
    using program_t = std::string;
    using expected_t = SimulationResult::State;
    auto test_cases = std::vector<std::pair<program_t, expected_t>>{
        { { "version 3.0; qubit q; pow(1./2).X q",
              SimulationResult::State{ { "0", { 0.5, 0.5, 0.5 } }, { "1", { 0.5, -0.5, 0.5 } } } },
         { "version 3.0; qubit q; pow(1./4).X q",
                SimulationResult::State{
                    { "0", { (2.0 + gates::SQRT_2) / 4, gates::SQRT_2 / 4, (2.0 + gates::SQRT_2) / 4 } },
                    { "1", { (2.0 - gates::SQRT_2) / 4, -gates::SQRT_2 / 4, (2.0 - gates::SQRT_2) / 4 } } } },
         { "version 3.0; qubit q; pow(1./2).H q",
                SimulationResult::State{ { "0", { (2.0 + gates::SQRT_2) / 4, (2.0 - gates::SQRT_2) / 4, 0.75 } },
                    { "1", { gates::SQRT_2 / 4, -gates::SQRT_2 / 4, 0.25 } } } },
         { "version 3.0; qubit q; pow(1./4).H q",
                SimulationResult::State{
                    { "0", { (1.0 + 2 * gates::SQRT_2) / 4, (-1.0 + gates::SQRT_2) / 4, (6.0 + gates::SQRT_2) / 8 } },
                    { "1", { (-1.0 + gates::SQRT_2) / 4, -0.25, (2.0 - gates::SQRT_2) / 8 } } } },
         { "version 3.0; qubit q; pow(pi/10).H q",
                SimulationResult::State{ { "0", { 0.934278931480042, 0.122188364647641, 0.887807118262753 } },
                    { "1", { 0.158664694954535, -0.294988807096525, 0.112192881737247 } } } },
         { "version 3.0; qubit q; pow(1./2).H q; pow(pi/4).X q",
                SimulationResult::State{ { "0", { 0.252183929405727, -0.142789875460971, 0.0839856827846723 } },
                    { "1", { 0.954922851780821, -0.0643169057255765, 0.916014317215328 } } } },
         { "version 3.0; qubit q; X q; pow(1./2).Z q", SimulationResult::State{ { "1", { 0, 1, 1 } } } },
         { "version 3.0; qubit[2] q; X q[0]; ctrl.pow(1./3).H q[0], q[1]",
                SimulationResult::State{
                    { "01",
                        { (6.0 + gates::SQRT_2) / 8, (2 * gates::SQRT_3 - gates::SQRT_2 * gates::SQRT_3) / 8, 0.875 } },
                    { "11", { 1.0 / (4 * gates::SQRT_2), -(gates::SQRT_3 / gates::SQRT_2) / 4, 0.125 } } } } }
    };

    std::size_t iterations = 1;
    for (const auto& [program, expected] : test_cases) {
        auto actual = run_from_string(program, iterations, "3.0");
        EXPECT_EQ(actual.state, expected);
    }
}

}  // namespace qx
