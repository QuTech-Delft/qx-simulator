#pragma once

#include <algorithm>  // for_each
#include <memory>
#include <string>
#include <vector>

#include "qx/circuit.hpp"
#include "qx/operands_helper.hpp"
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
    void visit_node(CqasmV3xNode&) override;
    void visit_gate_instruction(CqasmV3xGateInstruction& gate_instruction) override;
    void visit_non_gate_instruction(CqasmV3xNonGateInstruction& non_gate_instruction) override;

    std::vector<std::shared_ptr<Unitary>> get_gates(const CqasmV3xGate& gate, const CqasmV3xOperands& operands);
    std::vector<std::shared_ptr<Unitary>> get_modified_gates(
        const CqasmV3xGate& gate, const CqasmV3xOperands& operands);
    std::vector<std::shared_ptr<Unitary>> get_default_gates(const CqasmV3xGate& gate, const CqasmV3xOperands& operands);

private:
    Circuit& circuit_;
};

}  // namespace qx
