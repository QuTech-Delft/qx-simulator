#include "qx/Random.hpp"
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

bool operator==(Complex const &left, Complex const &right) {
    return std::abs(left.real - right.real) < config::EPS &&
        std::abs(left.imag - right.imag) < config::EPS &&
        std::abs(left.norm - right.norm) < config::EPS;
}

TEST_F(IntegrationTest, bell_pair) {
    auto cqasm = R"(
version 3.0

qubit[2] q

H q[0]
CNOT q[0], q[1]
)";
    auto actual = runFromString(cqasm, 1, "3.0");

    EXPECT_EQ(actual.shots_requested, 1);
    EXPECT_EQ(actual.shots_done, 1);
    EXPECT_EQ(actual.results, (SimulationResult::Results{ { "00", 1 } }));
    EXPECT_EQ(actual.state,
        (SimulationResult::State{
            { "00", Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } },
            { "11", Complex{ .real = 1 / std::sqrt(2), .imag = 0, .norm = 0.5 } }
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

    EXPECT_EQ(actual.state, (SimulationResult::State{ { "111111", Complex{ .real = 1, .imag = 0, .norm = 1 } } }));
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
    EXPECT_TRUE(std::holds_alternative<SimulationError>(result));
    EXPECT_THAT(std::get_if<SimulationError>(&result)->message, ::testing::StartsWith("""\
Cannot parse and analyze cQASM v3:\n\
Error at <unknown file name>:6:6..7: missing ']' at '\\n'"""));
}

TEST_F(IntegrationTest, measure) {
    std::size_t iterations = 10'000;
    auto cqasm = R"(
version 3.0

qubit[3] q

X q[0]
H q[1]
CNOT q[1], q[2]

measure q
)";
    auto actual = runFromString(cqasm, iterations);

    std::cout << "Simulation result: " << actual << "\n";

    auto error = static_cast<std::uint64_t>(iterations/2 * 0.05);
    EXPECT_EQ(actual.results.size(), 2);
    EXPECT_EQ(actual.results[0].first, "001");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[0].second)), error);
    EXPECT_EQ(actual.results[1].first, "111");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[1].second)), error);

    // State could be 001 or 111
    EXPECT_TRUE(actual.state[0].first.ends_with('1'));
    EXPECT_EQ(actual.state[0].second, (Complex{ .real = 1, .imag = 0, .norm = 1 }));
}

TEST_F(IntegrationTest, multiple_measure_instructions) {
    std::size_t iterations = 10'000;
    auto cqasm = R"(
version 3.0

qubit[3] q

X q[0]
H q[1]
CNOT q[1], q[2]

measure q[0]
measure q[1]
measure q[2]
)";
    auto actual = runFromString(cqasm, iterations);

    auto error = static_cast<std::uint64_t>(iterations/2 * 0.05);
    EXPECT_EQ(actual.results.size(), 2);
    EXPECT_EQ(actual.results[0].first, "001");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[0].second)), error);
    EXPECT_EQ(actual.results[1].first, "111");
    EXPECT_LT(std::abs(static_cast<long long>(iterations/2 - actual.results[1].second)), error);

    // State could be 001 or 111
    EXPECT_TRUE(actual.state[0].first.ends_with('1'));
    EXPECT_EQ(actual.state[0].second, (Complex{ .real = 1, .imag = 0, .norm = 1 }));
}

} // namespace qx
