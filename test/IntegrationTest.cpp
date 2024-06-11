#include "qx/Simulator.hpp"

#include <cmath>  // abs
#include <gtest/gtest.h>
#include <gmock/gmock.h>


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
    auto actual = runFromString(cqasm, 1, "3.0");

    EXPECT_EQ(actual.shotsRequested, 1);
    EXPECT_EQ(actual.shotsDone, 1);
    EXPECT_EQ(actual.states,
        (SimulationResult::States{
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
    auto actual = runFromString(cqasm, 2);

    EXPECT_EQ(actual.states,
              (SimulationResult::States{ { "111111", core::Complex{ .real = 1, .imag = 0, .norm = 1 } } }));
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
    auto actual = runFromString(cqasm, 1, "3.0");

    EXPECT_EQ(actual.shotsRequested, 1);
    EXPECT_EQ(actual.shotsDone, 1);
    EXPECT_EQ(actual.states,
        (SimulationResult::States{
            { "00", core::Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } },
            { "11", core::Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } }
    }));
}

TEST_F(IntegrationTest, measure) {
    std::size_t iterations = 10'000;
    auto cqasm = R"(
version 3.0

qubit[3] q
bit[3] b

X q[0]
H q[1]
CNOT q[1], q[2]

b = measure q
)";
    auto actual = runFromString(cqasm, iterations);

    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.results.size(), 2);
    EXPECT_EQ(actual.results[0].state, "001");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[0].count)), error);
    EXPECT_EQ(actual.results[1].state, "111");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[1].count)), error);

    // State could be 001 or 111
    EXPECT_TRUE(actual.states[0].value.ends_with('1'));
    EXPECT_EQ(actual.states[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));
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

    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.results.size(), 2);
    EXPECT_EQ(actual.results[0].state, "001");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[0].count)), error);
    EXPECT_EQ(actual.results[1].state, "111");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[1].count)), error);

    // State could be 001 or 111
    EXPECT_TRUE(actual.states[0].value.ends_with('1'));
    EXPECT_EQ(actual.states[0].amplitude, (core::Complex{ .real = 1, .imag = 0, .norm = 1 }));
}

TEST_F(IntegrationTest, mid_circuit_measure_instruction) {
    std::size_t iterations = 10'000;
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
    auto actual = runFromString(cqasm, iterations);

    // Expected output state: |00>+|11> or |01>+|10>
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.results.size(), 2);
    EXPECT_TRUE(actual.results[0].state == "00" || actual.results[0].state == "01");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[0].count)), error);
    EXPECT_TRUE(actual.results[1].state == "11" || actual.results[1].state == "10");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[1].count)), error);
}

TEST_F(IntegrationTest, multiple_qubit_bit_definitions_and_mid_circuit_measure_instructions) {
    std::size_t iterations = 10'000;
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
    auto actual = runFromString(cqasm, iterations);

    // Expected output state: |00>+|11> or |01>+|10>
    auto error = static_cast<std::uint64_t>(static_cast<double>(iterations)/2 * 0.05);
    EXPECT_EQ(actual.results.size(), 2);
    EXPECT_TRUE(actual.results[0].state == "00" || actual.results[0].state == "01");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[0].count)), error);
    EXPECT_TRUE(actual.results[1].state == "11" || actual.results[1].state == "10");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[1].count)), error);
}

} // namespace qx
