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
    for (const auto& instruction : get_gates(*gate_instruction.gate, gate_instruction.operands)) {
        circuit_.add_instruction(instruction);
    }
}

std::vector<std::shared_ptr<Unitary>> CircuitBuilder::get_gates(
    const CqasmV3xGate& gate, const CqasmV3xOperands& operands) {
    if (gate.name == "inv" || gate.name == "pow" || gate.name == "ctrl") {
        return get_modified_gates(gate, operands);
    } else {
        return get_default_gates(gate, operands);
    }
}

std::vector<std::shared_ptr<Unitary>> CircuitBuilder::get_modified_gates(
    const CqasmV3xGate& gate, const CqasmV3xOperands& operands) {
    const auto& modified_gates = get_gates(*gate.gate, operands);
    auto ret = std::vector<std::shared_ptr<Unitary>>(modified_gates.size());
    std::transform(modified_gates.begin(), modified_gates.end(), ret.begin(), [&gate](const auto& modified_gate) {
        if (gate.name == "inv") {
            modified_gate->matrix = modified_gate->inverse();
        } else if (gate.name == "pow") {
            auto exponent = gate.parameters[0]->as_const_float()->value;
            modified_gate->matrix = modified_gate->power(exponent);
        } else if (gate.name == "ctrl") {
            modified_gate->matrix = modified_gate->control();
        }
        return modified_gate;
    });
    return ret;
}

std::vector<std::shared_ptr<Unitary>> CircuitBuilder::get_default_gates(
    const CqasmV3xGate& gate, const CqasmV3xOperands& operands) {
    try {
        const auto& matrix_generator = gates::default_gates[gate.name];
        const auto& matrix = matrix_generator(gate.parameters);
        const auto& instructions_indices = get_instructions_indices(operands);
        auto ret = std::vector<std::shared_ptr<Unitary>>(instructions_indices.size());
        std::transform(instructions_indices.begin(),
            instructions_indices.end(),
            ret.begin(),
            [&matrix](const auto& instruction_indices) {
                return std::make_shared<Unitary>(
                    std::make_shared<core::matrix_t>(matrix), std::make_shared<core::operands_t>(instruction_indices));
            });
        return ret;
    } catch (const std::exception&) {
        throw CircuitBuilderError{ fmt::format("unknown default gate: '{}'", gate.name) };
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
        for (const auto& instruction_indices : instructions_indices) {
            const auto& qubit_index = instruction_indices[0];
            circuit_.add_instruction(std::make_shared<Reset>(qubit_index));
        }
    } else if (name == "init") {
        for (const auto& instruction_indices : instructions_indices) {
            const auto& qubit_index = instruction_indices[0];
            const auto& register_manager = RegisterManager::get_instance();
            if (register_manager.is_dirty_qubit(qubit_index.value)) {
                const auto& variable_name = register_manager.get_qubit_variable_name(qubit_index.value);
                const auto& variable_index = register_manager.get_qubit_variable_index(qubit_index.value);
                throw CircuitBuilderError{ fmt::format(
                    "incorrect 'init {}[{}]': the qubit has already been used in a non-control instruction",
                    variable_name,
                    variable_index) };
            }
        }
    } else if (name == "barrier") {
    } else if (name == "wait") {
        auto time = non_gate_instruction.parameters[0]->as_const_int()->value;
        if (time < 0) {
            const auto& instruction_indices = instructions_indices[0];
            const auto& qubit_index = instruction_indices[0];
            const auto& register_manager = RegisterManager::get_instance();
            const auto& variable_name = register_manager.get_qubit_variable_name(qubit_index.value);
            const auto& variable_index = register_manager.get_qubit_variable_index(qubit_index.value);
            throw CircuitBuilderError{ fmt::format(
                "incorrect 'wait({}) {}[{}]': time cannot be negative", time, variable_name, variable_index) };
        }
    } else {
        throw CircuitBuilderError{ fmt::format("unsupported non-gate instruction: '{}'", name) };
    }
}

}  // namespace qx
