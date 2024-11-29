#include "qx/Simulator.hpp"

#include <cmath>  // abs
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>  // nullopt


namespace qx {

class IntegrationTest : public ::testing::Test {
public:
    static SimulationResult runFromString(
        const std::string &s, std::uint64_t iterations = 1, std::string cqasm_version = "3.0") {

        auto result = executeString(s, iterations, std::nullopt, std::move(cqasm_version));
        EXPECT_TRUE(std::holds_alternative<SimulationResult>(result));
        return *std::get_if<SimulationResult>(&result);
    }
};


TEST_F(IntegrationTest, bell_pair) {
    auto cqasm = R"(
version 3.0

qubit[2] q

H q[0]
CNOT q[0], q[1]
)";
    std::size_t iterations = 1;
    auto actual = runFromString(cqasm, iterations, "3.0");

    EXPECT_EQ(actual.shotsRequested, iterations);
    EXPECT_EQ(actual.shotsDone, iterations);

    // Expected q state should be |00>+|11>
    // State is |00>+|11> after creating the Bell state
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } },
            { "11", core::Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, range_operands) {
    auto cqasm = R"(
version 3.0

qubit[6] q
X q[0:2]
CNOT q[0:2], q[3:5]
)";
    std::size_t iterations = 2;
    auto actual = runFromString(cqasm, iterations);

    // Expected q state should be |111111>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{ { "111111", core::Complex{ .real = 1, .imag = 0, .norm = 1 } } }));
}

TEST_F(IntegrationTest, too_many_qubits) {
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(executeString("version 3.0; qubit[62] q")));
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(executeString("version 3.0; qubit[63] q")));
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(executeString("version 3.0; qubit[64] q")));

    EXPECT_TRUE(std::holds_alternative<SimulationError>(executeString("version 3.0; qubit[65] q")));
    EXPECT_TRUE(std::holds_alternative<SimulationError>(executeString("version 3.0; qubit[66] q")));
}

TEST_F(IntegrationTest, syntax_error) {
    auto cqasm = R"(
version 3.0

qubit q

H q[0
)";

    auto result = executeString(cqasm);
    auto message = std::string{ std::get_if<SimulationError>(&result)->what() };
    EXPECT_TRUE(std::holds_alternative<SimulationError>(result));
    EXPECT_THAT(message, ::testing::StartsWith("""\
Cannot parse and analyze cQASM v3:\n\
Error at <unknown file name>:6:6..7: missing ']' at '\\n'"""));
}

TEST_F(IntegrationTest, identity) {
    auto cqasm = R"(
version 3.0

qubit[2] q

H q[0]
I q[0]
CNOT q[0], q[1]
I q[1]
)";
    std::size_t iterations = 1;
    auto actual = runFromString(cqasm, iterations, "3.0");

    EXPECT_EQ(actual.shotsRequested, iterations);
    EXPECT_EQ(actual.shotsDone, iterations);

    // Expected q state should be |00>+|11>
    // State is |00>+|11> after creating the Bell state
    // Identity gates do not modify the state of the qubits
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } },
            { "11", core::Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, measure) {
    auto cqasm = R"(
version 3.0

qubit[3] q
bit[3] b

X q[0]
H q[1]
CNOT q[1], q[2]

b = measure q
)";
    std::size_t iterations = 10'000;
    auto actual = runFromString(cqasm, iterations);

    // Expected q state should be |001> or |111>
    EXPECT_TRUE(actual.state[0].value.ends_with('1'));
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected b value should be "001" 50% of the cases and "111" 50% of the cases
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_EQ(actual.measurements[0].state, "001");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[0].count)), error);
    EXPECT_EQ(actual.measurements[1].state, "111");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[1].count)), error);
}

TEST_F(IntegrationTest, multiple_measure_instructions) {
    auto cqasm = R"(
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
    auto actual = runFromString(cqasm, iterations);

    // Expected q state should be |001> or |111>
    EXPECT_TRUE(actual.state[0].value.ends_with('1'));
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected b value should be "001" 50% of the cases and "111" 50% of the cases
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_EQ(actual.measurements[0].state, "001");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[0].count)), error);
    EXPECT_EQ(actual.measurements[1].state, "111");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[1].count)), error);
}

TEST_F(IntegrationTest, mid_circuit_measure_instruction) {
    auto cqasm = R"(
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
    auto actual = runFromString(cqasm, iterations);

    // Expected q state should be |00>+|11> or |01>+|10>
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_TRUE(actual.measurements[0].state == "00" || actual.measurements[0].state == "01");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[0].count)), error);
    EXPECT_TRUE(actual.measurements[1].state == "11" || actual.measurements[1].state == "10");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[1].count)), error);
}

TEST_F(IntegrationTest, multiple_qubit_bit_definitions_and_mid_circuit_measure_instructions) {
    auto cqasm = R"(
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
    auto actual = runFromString(cqasm, iterations);

    // Expected q1-q0 state should be |00>+|11> or |01>+|10>
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_TRUE(actual.measurements[0].state == "00" || actual.measurements[0].state == "01");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[0].count)), error);
    EXPECT_TRUE(actual.measurements[1].state == "11" || actual.measurements[1].state == "10");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[1].count)), error);
}

TEST_F(IntegrationTest, reset__x_measure_reset) {
    auto cqasm = R"(
version 3.0

qubit q
bit b
X q
b = measure q
reset q
)";
    std::size_t iterations = 10'000;
    auto actual = runFromString(cqasm, iterations);

    // Expected q state should always be |0> because reset modifies the qubit state
    EXPECT_EQ(actual.state[0].value, "0");
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected b value should always be "1" because reset does not modify the measurement register
    EXPECT_EQ(actual.measurements.size(), 1);
    EXPECT_TRUE(actual.measurements[0].state == "1");
    EXPECT_EQ(actual.measurements[0].count, iterations);
}

TEST_F(IntegrationTest, reset__bell_state_then_reset) {
    std::size_t iterations = 10'000;
    auto cqasm = R"(
version 3.0

qubit[2] q
bit[2] b
H q[0]
CNOT q[0], q[1]
reset q[0]
)";
    auto actual = runFromString(cqasm, iterations);

    // Expected q state should be |00>+|10>
    // State is |00>+|11> after creating the Bell state
    // Then reset sets q to |0>, leaving the state as |00>+|10>
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", core::Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } },
            { "10", core::Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } }
        }));
}

TEST_F(IntegrationTest, reset__bell_state_then_reset_and_measure) {
    std::size_t iterations = 10'000;
    auto cqasm = R"(
version 3.0

qubit[2] q
bit[2] b
H q[0]
CNOT q[0], q[1]
reset q[0]
b = measure q
)";
    auto actual = runFromString(cqasm, iterations);

    // Expected q state should be |00>+|10>
    // State is |00>+|11> after creating the Bell state
    // Then reset sets q to |0>, leaving the state as |00>+|10>
    // The measure provokes a collapse of the state to either |00> or |10>
    EXPECT_TRUE(actual.state[0].value.ends_with('0'));
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected b value should be "00" 50% of the cases and "10" 50% of the cases
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_TRUE(actual.measurements[0].state == "00" || actual.measurements[0].state == "10");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[0].count)), error);
}

TEST_F(IntegrationTest, reset__bell_state_then_measure_and_reset) {
    std::size_t iterations = 10'000;
    auto cqasm = R"(
version 3.0

qubit[2] q
bit[2] b
H q[0]
CNOT q[0], q[1]
b = measure q
reset q[0]
)";
    auto actual = runFromString(cqasm, iterations);

    // Expected q state should be |00> or |10>
    // State is |00>+|11> after creating the Bell state
    // The measure provokes a collapse of the state to either |00> or |11>
    // Then reset sets q0 to |0>, leaving the state as |00> or |10>
    EXPECT_TRUE(actual.state[0].value.ends_with('0'));
    EXPECT_EQ(actual.state[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));

    // Expected b value should be "00" 50% of the cases and "11" 50% of the cases
    // because the measure happens right after creating the Bell state,
    // and reset does not modify the measurement register
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.measurements.size(), 2);
    EXPECT_TRUE(actual.measurements[0].state == "00" || actual.measurements[0].state == "11");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.measurements[0].count)), error);
}

TEST_F(IntegrationTest, bit_measurement_register) {
    auto cqasm = R"(
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
    auto actual = runFromString(cqasm, iterations);

    // Expected bb[0] value should always be "1"
    // Expected b value should be "0" 50% of the cases and "1" 50% of the cases
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.bitMeasurements.size(), 2);
    for (auto const& bitMeasurement : actual.bitMeasurements) {
        EXPECT_EQ(actual.getBitMeasurement(bitMeasurement.state, "bb", 0), 1);
        EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - bitMeasurement.count)), error);
    }
}

} // namespace qx
