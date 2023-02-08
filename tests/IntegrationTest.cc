#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/Simulator.h"
#include "doctest/doctest.h"

namespace qx {

class IntegrationTest {
public:
    SimulationResult runFromString(std::string s, std::uint64_t iterations = 1) {
        auto simulationResult = executeString(s, iterations);
        REQUIRE(simulationResult);
        return *simulationResult;
    }

private:
    Simulator simulator;
};

bool operator==(SimulationResult::Complex const& left, SimulationResult::Complex const& right) {
    return std::abs(left.real - right.real) < config::EPS && std::abs(left.imag - right.imag) < config::EPS && std::abs(left.norm - right.norm) < config::EPS;
}

TEST_CASE_FIXTURE(IntegrationTest, "Bell pair") {
    auto cqasm = R"(
version 1.0

qubits 2

h q[0]
cnot q[0], q[1]
)";
    auto actual = runFromString(cqasm);

    CHECK_EQ(actual.shots_requested, 1);
    CHECK_EQ(actual.shots_done, 1);
    CHECK_EQ(actual.results, SimulationResult::Results{{"00", 1}});
    CHECK_EQ(actual.state, SimulationResult::State{{"00", {1/std::sqrt(2), 0, 0.5}}, {"11", {1/std::sqrt(2), 0, 0.5}}});
}

TEST_CASE_FIXTURE(IntegrationTest, "Controlled gates") {
    auto cqasm = R"(
version 1.0

qubits 2

cond(b[0]) h q[0]
x q[0:1]
measure q[0:1]
cond(b[0]) h q[0]
)";

    auto actual = runFromString(cqasm, 2);

    CHECK_EQ(actual.results, SimulationResult::Results{{"11", 2}});
    CHECK_EQ(actual.state, SimulationResult::State{{"10", {1/std::sqrt(2), 0, 0.5}}, {"11", {-1/std::sqrt(2), 0, 0.5}}});
}

TEST_CASE_FIXTURE(IntegrationTest, "Range operands") {
    auto cqasm = R"(
version 1.0

qubits 6
x q[0:2]
cnot q[0:2], q[3:5]
)";

    auto actual = runFromString(cqasm, 2);

    CHECK_EQ(actual.state, SimulationResult::State{{"111111", {1, 0, 1}}});
}

TEST_CASE_FIXTURE(IntegrationTest, "Classical not gate") {
    auto cqasm = R"(
version 1.0

qubits 1

x q[0]
measure q[0]
not b[0]
)";
    auto actual = runFromString(cqasm);

    CHECK_EQ(actual.results, SimulationResult::Results{{"0", 1}});
    CHECK_EQ(actual.state, SimulationResult::State{{"1", {1, 0, 1}}});
}

TEST_CASE_FIXTURE(IntegrationTest, "Shor-9 error correction code with bit flip") {
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

    CHECK_EQ(actual.results, SimulationResult::Results{{"00001000000000000", iterations}}); // Error syndrom
    CHECK_EQ(actual.state, SimulationResult::State{
        {"00001000000000011", {1/std::sqrt(8), 0, 0.125}},
        {"00001000000000100", {1/std::sqrt(8), 0, 0.125}},
        {"00001000000111011", {1/std::sqrt(8), 0, 0.125}},
        {"00001000000111100", {1/std::sqrt(8), 0, 0.125}},
        {"00001000111000011", {1/std::sqrt(8), 0, 0.125}},
        {"00001000111000100", {1/std::sqrt(8), 0, 0.125}},
        {"00001000111111011", {1/std::sqrt(8), 0, 0.125}},
        {"00001000111111100", {1/std::sqrt(8), 0, 0.125}}});
}

}