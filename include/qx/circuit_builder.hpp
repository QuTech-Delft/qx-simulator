#pragma once

#include <algorithm>  // for_each
#include <memory>
#include <string>
#include <vector>

#include "qx/circuit.hpp"
#include "qx/cqasm_v3x.hpp"
#include "qx/simulation_error.hpp"

namespace qx {

/*
 * CircuitBuilderError
 */
struct CircuitBuilderError : public SimulationError {
    explicit CircuitBuilderError(const std::string& message)
    : SimulationError{ message } {}
};

/*
 * CircuitBuilder
 */
class CircuitBuilder : public CqasmV3xRecursiveVisitor {
public:
    explicit CircuitBuilder(Circuit& circuit);
    Circuit build();

private:
    template <std::size_t N>
    using cqasm_v3x_operands_indices_t = std::array<CqasmV3xIndices, N>;

private:
    void visit_node(CqasmV3xNode&) override;
    void visit_gate_instruction(CqasmV3xGateInstruction& gate_instruction) override;
    void visit_non_gate_instruction(CqasmV3xNonGateInstruction& non_gate_instruction) override;
    std::vector<std::shared_ptr<Instruction>> get_gates(const CqasmV3xGate& gate, const CqasmV3xOperands& operands);
    template <std::size_t NumberOfQubitOperands>
    std::vector<std::shared_ptr<Instruction>> get_gates(core::matrix_t<NumberOfQubitOperands> matrix,
        cqasm_v3x_operands_indices_t<NumberOfQubitOperands> operands_indices);

private:
    Circuit& circuit_;
};

}  // namespace qx
