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
};

std::vector<std::shared_ptr<Instruction>> CircuitBuilder::get_gates(
    const CqasmV3xGate& gate, const CqasmV3xOperands& operands) {
    const auto& name = gate.name;
    if (name == "inv") {
        throw CircuitBuilderError{ "unimplemented: inv gate modifier" };
    } else if (name == "pow") {
        throw CircuitBuilderError{ "unimplemented: pow gate modifier" };
    } else if (name == "ctrl") {
        throw CircuitBuilderError{ "unimplemented: ctrl gate modifier" };
    } else if (name == "TOFFOLI") {
        return get_gates<3>(gates::TOFFOLI,
            { get_operand_indices(*operands[0]),
                get_operand_indices(*operands[1]),
                get_operand_indices(*operands[2]) });
    } else if (name == "I") {
        return get_gates<1>(gates::IDENTITY, { get_operand_indices(*operands[0]) });
    } else if (name == "X") {
        return get_gates<1>(gates::X, { get_operand_indices(*operands[0]) });
    } else if (name == "Y") {
        return get_gates<1>(gates::Y, { get_operand_indices(*operands[0]) });
    } else if (name == "Z") {
        return get_gates<1>(gates::Z, { get_operand_indices(*operands[0]) });
    } else if (name == "H") {
        return get_gates<1>(gates::H, { get_operand_indices(*operands[0]) });
    } else if (name == "S") {
        return get_gates<1>(gates::S, { get_operand_indices(*operands[0]) });
    } else if (name == "Sdag") {
        return get_gates<1>(gates::SDAG, { get_operand_indices(*operands[0]) });
    } else if (name == "T") {
        return get_gates<1>(gates::T, { get_operand_indices(*operands[0]) });
    } else if (name == "Tdag") {
        return get_gates<1>(gates::TDAG, { get_operand_indices(*operands[0]) });
    } else if (name == "Rx") {
        return get_gates<1>(gates::RX(gate.parameter->as_const_float()->value), { get_operand_indices(*operands[0]) });
    } else if (name == "Ry") {
        return get_gates<1>(gates::RY(gate.parameter->as_const_float()->value), { get_operand_indices(*operands[0]) });
    } else if (name == "Rz") {
        return get_gates<1>(gates::RZ(gate.parameter->as_const_float()->value), { get_operand_indices(*operands[0]) });
    } else if (name == "CNOT") {
        return get_gates<2>(gates::CNOT, { get_operand_indices(*operands[0]), get_operand_indices(*operands[1]) });
    } else if (name == "CZ") {
        return get_gates<2>(gates::CZ, { get_operand_indices(*operands[0]), get_operand_indices(*operands[1]) });
    } else if (name == "CR") {
        return get_gates<2>(gates::CR(gate.parameter->as_const_float()->value),
            { get_operand_indices(*operands[0]), get_operand_indices(*operands[1]) });
    } else if (name == "CRk") {
        return get_gates<2>(
            gates::CR(static_cast<double>(gates::PI) / std::pow(2, gate.parameter->as_const_int()->value) - 1),
            { get_operand_indices(*operands[0]), get_operand_indices(*operands[1]) });
    } else if (name == "SWAP") {
        return get_gates<2>(gates::SWAP, { get_operand_indices(*operands[0]), get_operand_indices(*operands[1]) });
    } else if (name == "X90") {
        return get_gates<1>(gates::X90, { get_operand_indices(*operands[0]) });
    } else if (name == "Y90") {
        return get_gates<1>(gates::Y90, { get_operand_indices(*operands[0]) });
    } else if (name == "mX90") {
        return get_gates<1>(gates::MX90, { get_operand_indices(*operands[0]) });
    } else if (name == "mY90") {
        return get_gates<1>(gates::MY90, { get_operand_indices(*operands[0]) });
    } else {
        throw CircuitBuilderError{ fmt::format("unsupported gate instruction: '{}'", name) };
    }
}

void CircuitBuilder::visit_non_gate_instruction(CqasmV3xNonGateInstruction& non_gate_instruction) {
    const auto& name = non_gate_instruction.instruction_ref->name;
    const auto& operands = non_gate_instruction.operands;

    if (name == "measure") {
        // A measure statement has the following syntax: b = measure q
        // The left-hand side operand, b, is the operand 0
        // The right-hand side operand, q, is the operand 1
        const auto& bit_indices = get_operand_indices(*operands[0]);
        const auto& qubit_indices = get_operand_indices(*operands[1]);
        for (size_t i{ 0 }; i < qubit_indices.size(); ++i) {
            circuit_.add_instruction(
                std::make_shared<Measure>(core::QubitIndex{ static_cast<std::size_t>(qubit_indices[i]->value) },
                    core::BitIndex{ static_cast<std::size_t>(bit_indices[i]->value) }));
        }
    } else if (name == "reset") {
        if (operands.empty()) {
            circuit_.add_instruction(std::make_shared<Reset>(std::nullopt));
        } else {
            const auto& qubit_indices = get_operand_indices(*operands[0]);
            for (size_t i{ 0 }; i < qubit_indices.size(); ++i) {
                circuit_.add_instruction(
                    std::make_shared<Reset>(core::QubitIndex{ static_cast<std::size_t>(qubit_indices[i]->value) }));
            }
        }
    } else {
        throw CircuitBuilderError{ fmt::format("unsupported non-gate instruction: '{}'", name) };
    }
}

template <std::size_t NumberOfQubitOperands>
std::vector<std::shared_ptr<Instruction>> CircuitBuilder::get_gates(core::matrix_t<NumberOfQubitOperands> matrix,
    cqasm_v3x_operands_indices_t<NumberOfQubitOperands> operands_indices) {
    static_assert(NumberOfQubitOperands > 0);

#ifndef NDEBUG
    // SGMQ check: all the operands use the same number of indices
    std::for_each(operands_indices.begin(),
        operands_indices.end(),
        [&operands_indices](const CqasmV3xIndices& indices) { assert(indices.size() == operands_indices[0].size()); });
#endif

    // SGMQ unrolling: add one instruction for each operand's index
    std::vector<std::shared_ptr<Instruction>> ret{ operands_indices[0].size() };
    for (std::size_t i = 0; i < operands_indices[0].size(); ++i) {
        core::operands_t<NumberOfQubitOperands> operands{};
        for (std::size_t qubit_index = 0; qubit_index < NumberOfQubitOperands; ++qubit_index) {
            operands[qubit_index] =
                core::QubitIndex{ static_cast<std::size_t>(operands_indices[qubit_index][i]->value) };
        }
        ret[i] = std::make_shared<Unitary<NumberOfQubitOperands>>(matrix, operands);
    }
    return ret;
}

}  // namespace qx
