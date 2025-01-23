#include "qx/circuit_builder.hpp"

#include <fmt/format.h>

#include <algorithm>  // for_each
#include <memory>  // make_shared

#include "qx/core.hpp"
#include "qx/gates.hpp"
#include "qx/instructions.hpp"  // Measure, Unitary
#include "qx/operands_helper.hpp"

namespace qx {

CircuitBuilder::CircuitBuilder(Circuit& circuit)
: circuit_{ circuit } {}

Circuit CircuitBuilder::build() {
    const auto& statements = circuit_.program->block->statements;
    std::for_each(statements.begin(), statements.end(), [this](const auto& statement) { statement->visit(*this); });
    return circuit_;
}

void CircuitBuilder::visit_node(CqasmV3xNode&) {
    throw CircuitBuilderError{ "unsupported node" };
}

void CircuitBuilder::visit_gate_instruction(CqasmV3xGateInstruction& gate_instruction) {
    const auto& name = gate_instruction.instruction_ref->name;
    const auto& operands = gate_instruction.operands;

    for (const auto& instruction : get_gates(name, operands)) {
        circuit_.add_instruction(instruction);
    }
}

std::vector<std::shared_ptr<Unitary>> CircuitBuilder::get_gates(
    const CqasmV3xGate& gate, const CqasmV3xOperands& operands) {
    if (gate.name == "inv" || gate.name == "pow" || gate.name == "ctrl") {
        return get_modified_gates(gate, get_gates(*gate.gate, operands));
    } else {
        return get_default_gates(gate, operands);
    }
}

std::vector<std::shared_ptr<Unitary>> CircuitBuilder::get_modified_gates(
    const CqasmV3xGate& gate, const std::vector<std::shared_ptr<Unitary>>& modified_gates) {
    auto ret = std::vector<std::shared_ptr<Unitary>>{};
    for (auto modified_gate : modified_gates) {
        if (gate.name == "inv") {
            modified_gate->matrix = modified_gate->inverse();
        } else if (gate.name == "pow") {
            auto exponent = gate.parameter->as_const_float()->value;
            modified_gate->matrix = modified_gate->power(exponent);
        } else if (gate.name == "ctrl") {
            modified_gate->matrix = modified_gate->control();
        }
    }
    return ret;
}

std::vector<std::shared_ptr<Unitary>> CircuitBuilder::get_default_gates(
    const CqasmV3xGate& gate, const CqasmV3xOperands& operands) {
    const auto& name = gate.name;
    try {
        const auto& matrix_generator = gates::default_gates[gate.name];
        const auto& matrix = matrix_generator(gate.parameter);
        const auto& instructions_indices = get_instructions_indices(operands);
        auto ret = std::vector<std::shared_ptr<Unitary>>(instructions_indices.size());
        std::transform(
            instructions_indices.begin(), instructions_indices.end(), ret.begin(), [&matrix](auto& unrolled_operand) {
                return std::make_shared<Unitary>(
                    std::make_shared<core::matrix_t>(matrix), std::make_shared<core::operands_t>(unrolled_operand));
            });
        return ret;
    } catch (const std::exception&) {
        throw CircuitBuilderError{ fmt::format("unknown default gate: '{}'", name) };
    }
}

void CircuitBuilder::visit_non_gate_instruction(CqasmV3xNonGateInstruction& non_gate_instruction) {
    const auto& name = non_gate_instruction.instruction_ref->name;
    const auto& operands = non_gate_instruction.operands;
    const auto& instructions_indices = get_instructions_indices(operands);

    if (name == "measure") {
        // A measure statement has the following syntax: b = measure q
        // The left-hand side operand, b, is the operand 0
        // The right-hand side operand, q, is the operand 1
        for (const auto& instruction_indices : instructions_indices) {
            const auto& bit_index = instruction_indices[0];
            const auto& qubit_index = instruction_indices[1];
            circuit_.add_instruction(std::make_shared<Measure>(qubit_index, bit_index));
        }
    } else if (name == "reset") {
        if (operands.empty()) {
            circuit_.add_instruction(std::make_shared<Reset>(std::nullopt));
        } else {
            for (const auto& instruction_indices : instructions_indices) {
                const auto& qubit_index = instruction_indices[0];
                circuit_.add_instruction(std::make_shared<Reset>(qubit_index));
            }
        }
    } else {
        throw CircuitBuilderError{ fmt::format("unsupported non-gate instruction: '{}'", name) };
    }
}

}  // namespace qx
