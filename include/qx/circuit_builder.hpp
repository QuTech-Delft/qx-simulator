#pragma once

#include "qx/circuit.hpp"
#include "qx/cqasm_v3x.hpp"
#include "qx/simulation_error.hpp"

#include <algorithm>  // for_each
#include <string>


namespace qx {

/*
 * CircuitBuilderError
 */
struct CircuitBuilderError : public SimulationError {
    explicit CircuitBuilderError(const std::string &message) : SimulationError{ message } {}
};

/*
 * CircuitBuilder
 */
class CircuitBuilder : public CqasmV3xRecursiveVisitor {
public:
    explicit CircuitBuilder(Circuit &circuit);
    Circuit build();

private:
    template <std::size_t N>
    using cqasm_v3x_operands_indices_t = std::array<CqasmV3xIndices, N>;

private:
    void visit_node(CqasmV3xNode &) override;
    void visit_instruction(CqasmV3xInstruction &instruction) override;
    template <std::size_t NumberOfQubitOperands>
    void visit_gate_instruction(
        core::matrix_t<NumberOfQubitOperands> matrix,
        cqasm_v3x_operands_indices_t<NumberOfQubitOperands> operands_indices);

private:
    Circuit &circuit_;
};

}  // namespace qx
