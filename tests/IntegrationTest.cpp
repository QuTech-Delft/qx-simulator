#include "qx/Random.hpp"
#include "qx/Simulator.hpp"

#include <cassert>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace qx {

class IntegrationTest : public ::testing::Test {
public:
    static SimulationResult runFromString(const std::string &s,
                                          std::uint64_t iterations = 1) {
        auto result = executeString(s, iterations);
        EXPECT_TRUE(std::holds_alternative<SimulationResult>(result));
        return *std::get_if<SimulationResult>(&result);
    }

private:
    Simulator simulator;
};

bool operator==(SimulationResult::Complex const &left,
                SimulationResult::Complex const &right) {
    return std::abs(left.real - right.real) < config::EPS &&
           std::abs(left.imag - right.imag) < config::EPS &&
           std::abs(left.norm - right.norm) < config::EPS;
}

TEST_F(IntegrationTest, bell_pair) {
    auto cqasm = R"(
version 1.0

qubits 2

h q[0]
cnot q[0], q[1]
)";
    auto actual = runFromString(cqasm);

    EXPECT_EQ(actual.shots_requested, 1);
    EXPECT_EQ(actual.shots_done, 1);
    EXPECT_EQ(actual.results, (SimulationResult::Results{{"00", 1}}));
    EXPECT_EQ(actual.state,
              (SimulationResult::State{{"00", {1 / std::sqrt(2), 0, 0.5}},
                                       {"11", {1 / std::sqrt(2), 0, 0.5}}}));
}

TEST_F(IntegrationTest, controlled_gates) {
    auto cqasm = R"(
version 1.0

qubits 2

cond(b[0]) h q[0]
x q[0:1]
measure q[0:1]
cond(b[0]) h q[0]
)";
    auto actual = runFromString(cqasm, 2);

    EXPECT_EQ(actual.results, (SimulationResult::Results{{"11", 2}}));
    EXPECT_EQ(actual.state,
              (SimulationResult::State{{"10", {1 / std::sqrt(2), 0, 0.5}},
                                       {"11", {-1 / std::sqrt(2), 0, 0.5}}}));
}

TEST_F(IntegrationTest, range_operands) {
    auto cqasm = R"(
version 1.0

qubits 6
x q[0:2]
cnot q[0:2], q[3:5]
)";
    auto actual = runFromString(cqasm, 2);

    EXPECT_EQ(actual.state, (SimulationResult::State{{"111111", {1, 0, 1}}}));
}

TEST_F(IntegrationTest, repeated_subcircuit) {
    auto cqasm = R"(
version 1.0

qubits 1

.subcircuit(3)
    x q[0]

.another_subcircuit(5)
    h q[0]
)";
    auto actual = runFromString(cqasm, 2);

    EXPECT_EQ(actual.state,
              (SimulationResult::State{{"0", {1 / std::sqrt(2), 0, 0.5}},
                                       {"1", {-1 / std::sqrt(2), 0, 0.5}}}));
}

TEST_F(IntegrationTest, classical_not_gate) {
    auto cqasm = R"(
version 1.0

qubits 1

x q[0]
measure q[0]
not b[0]
)";
    auto actual = runFromString(cqasm);

    EXPECT_EQ(actual.results, (SimulationResult::Results{{"0", 1}}));
    EXPECT_EQ(actual.state, (SimulationResult::State{{"1", {1, 0, 1}}}));
}

TEST_F(IntegrationTest, shor_9_error_correction_code_with_bit_flip) {
    auto cqasm = R"(
version 1.0

qubits 17
prep_z q[0:8]

.encode
CNOT q[0],q[3]
CNOT q[0],q[6]
{ H q[0] | H q[3] | H q[6]}
{ CNOT q[0],q[1] | CNOT q[3],q[4] | CNOT q[6],q[7] }
{ CNOT q[0],q[2] | CNOT q[3],q[5] | CNOT q[6],q[8] }

.error
X q[2]

.stabilizers
prep_z q[9:16]
H q[9:16]

CZ q[8],q[16]
CZ q[7],q[16]
CZ q[7],q[15]
CZ q[6],q[15]
CZ q[5],q[14]
CZ q[4],q[14]
CZ q[4],q[13]
CZ q[3],q[13]
CZ q[2],q[12]
CZ q[1],q[12]
CZ q[1],q[11]
CZ q[0],q[11]

CNOT q[10],q[8]
CNOT q[10],q[7]
CNOT q[10],q[6]
CNOT q[10],q[5]
CNOT q[10],q[4]
CNOT q[10],q[3]

CNOT q[9],q[5]
CNOT q[9],q[4]
CNOT q[9],q[3]
CNOT q[9],q[2]
CNOT q[9],q[1]
CNOT q[9],q[0]

H q[9:16]

measure_z q[9:16]
)";
    std::size_t iterations = 7;
    auto actual = runFromString(cqasm, iterations);

    EXPECT_EQ(actual.results,
              (SimulationResult::Results{
                  {"00001000000000000", iterations}}));  // Error syndrome
    EXPECT_EQ(actual.state,
              (SimulationResult::State{
                  {"00001000000000011", {1 / std::sqrt(8), 0, 0.125}},
                  {"00001000000000100", {1 / std::sqrt(8), 0, 0.125}},
                  {"00001000000111011", {1 / std::sqrt(8), 0, 0.125}},
                  {"00001000000111100", {1 / std::sqrt(8), 0, 0.125}},
                  {"00001000111000011", {1 / std::sqrt(8), 0, 0.125}},
                  {"00001000111000100", {1 / std::sqrt(8), 0, 0.125}},
                  {"00001000111111011", {1 / std::sqrt(8), 0, 0.125}},
                  {"00001000111111100", {1 / std::sqrt(8), 0, 0.125}}}));
}

TEST_F(IntegrationTest, too_many_qubits) {
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(executeString("version 1.0; qubits 62")));
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(executeString("version 1.0; qubits 63")));
    EXPECT_TRUE(std::holds_alternative<SimulationResult>(executeString("version 1.0; qubits 64")));
    EXPECT_TRUE(std::holds_alternative<SimulationError>(executeString("version 1.0; qubits 65")));
    EXPECT_TRUE(std::holds_alternative<SimulationError>(executeString("version 1.0; qubits 66")));
}

TEST_F(IntegrationTest, syntax_error) {
    auto cqasm = R"(
version 1.0

qubits 1

h q[0
)";

    auto result = executeString(cqasm);
    EXPECT_TRUE(std::holds_alternative<SimulationError>(result));
    EXPECT_THAT(std::get_if<SimulationError>(&result)->message, ::testing::StartsWith("""\
Cannot parse and analyze string \nversion 1.0\n\nqubits 1\n\nh q[0\n: \n\
<unknown>:6:6: syntax error, unexpected NEWLINE"""));
}

TEST_F(IntegrationTest, unknown_error_model) {
    auto cqasm = R"(
version 1.0

qubits 1

error_model amplitude_damping, 0.1
)";

    EXPECT_TRUE(std::holds_alternative<SimulationError>(executeString(cqasm)));
}

TEST_F(IntegrationTest, depolarizing_channel_with_constant_seed) {
    random::seed(1234);
    std::size_t iterations = 10000;
    auto cqasm = R"(
version 1.0

qubits 3

error_model depolarizing_channel, 0.1

x q[0]
measure_all
)";
    auto actual = runFromString(cqasm, iterations);

    EXPECT_EQ(actual.results, (SimulationResult::Results{{"000", 396},
                                                         {"001", 8759},
                                                         {"010", 10},
                                                         {"011", 438},
                                                         {"100", 10},
                                                         {"101", 380},
                                                         {"111", 7}}));
}

} // namespace qx
