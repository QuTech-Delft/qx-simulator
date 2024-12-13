#include "qx/circuit_builder.hpp"
#include "qx/core.hpp"
#include "qx/gates.hpp"
#include "qx/instructions.hpp" // Measure, Unitary
#include "qx/operands_helper.hpp"

#include <algorithm>  // for_each
#include <fmt/format.h>
#include <memory>  // make_shared


namespace qx {

CircuitBuilder::CircuitBuilder(Circuit &circuit)
    : circuit_{ circuit }
{}

Circuit CircuitBuilder::build() {
    auto const& statements = circuit_.program->block->statements;
    std::for_each(statements.begin(), statements.end(),
        [this](auto const& statement) {
            statement->visit(*this);
        });
    return circuit_;
}

void CircuitBuilder::visit_node(CqasmV3xNode &) {
    throw CircuitBuilderError{ "unsupported node" };
}

void CircuitBuilder::visit_instruction(CqasmV3xInstruction &instruction) {
    auto &name = instruction.instruction_ref->name;
    auto operands_helper = OperandsHelper{ instruction };
    
    if (name == "TOFFOLI") {
        visit_gate_instruction<3>(
            gates::TOFFOLI,
            {
                operands_helper.get_register_operand(0),
                operands_helper.get_register_operand(1),
                operands_helper.get_register_operand(2)
            }
        );
    } else if (name == "I") {
        visit_gate_instruction<1>(gates::IDENTITY, { operands_helper.get_register_operand(0) });
    } else if (name == "X") {
        visit_gate_instruction<1>(gates::X, { operands_helper.get_register_operand(0) });
    } else if (name == "Y") {
        visit_gate_instruction<1>(gates::Y, { operands_helper.get_register_operand(0) });
    } else if (name == "Z") {
        visit_gate_instruction<1>(gates::Z, { operands_helper.get_register_operand(0) });
    } else if (name == "H") {
        visit_gate_instruction<1>(gates::H, { operands_helper.get_register_operand(0) });
    } else if (name == "S") {
        visit_gate_instruction<1>(gates::S, { operands_helper.get_register_operand(0) });
    } else if (name == "Sdag") {
        visit_gate_instruction<1>(gates::SDAG, { operands_helper.get_register_operand(0) });
    } else if (name == "T") {
        visit_gate_instruction<1>(gates::T, { operands_helper.get_register_operand(0) });
    } else if (name == "Tdag") {
        visit_gate_instruction<1>(gates::TDAG, {operands_helper.get_register_operand(0) });
    } else if (name == "Rx") {
        visit_gate_instruction<1>(
            gates::RX(operands_helper.get_float_operand(1)),
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "Ry") {
        visit_gate_instruction<1>(
            gates::RY(operands_helper.get_float_operand(1)),
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "Rz") {
        visit_gate_instruction<1>(
            gates::RZ(operands_helper.get_float_operand(1)),
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "CNOT") {
        visit_gate_instruction<2>(
            gates::CNOT,
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "CZ") {
        visit_gate_instruction<2>(
            gates::CZ,
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "CR") {
        visit_gate_instruction<2>(
            gates::CR(operands_helper.get_float_operand(2)),
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "CRk") {
        visit_gate_instruction<2>(
            gates::CR(static_cast<double>(gates::PI) / std::pow(2, operands_helper.get_int_operand(2) - 1)),
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "SWAP") {
        visit_gate_instruction<2>(
            gates::SWAP,
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "X90") {
        return visit_gate_instruction<1>(
            gates::X90,
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "Y90") {
        return visit_gate_instruction<1>(
            gates::Y90,
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "mX90") {
        return visit_gate_instruction<1>(
            gates::MX90,
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "mY90") {
        return visit_gate_instruction<1>(
            gates::MY90,
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "measure") {
        // A measure statement has the following syntax: b = measure q
        // The left-hand side operand, b, is the operand 0
        // The right-hand side operand, q, is the operand 1
        const auto &bit_indices = operands_helper.get_register_operand(0);
        const auto &qubit_indices = operands_helper.get_register_operand(1);
        for (size_t i{ 0 }; i < qubit_indices.size(); ++i) {
            circuit_.add_instruction(std::make_shared<Measure>(
                core::QubitIndex{ static_cast<std::size_t>(qubit_indices[i]->value) },
                core::BitIndex{ static_cast<std::size_t>(bit_indices[i]->value) }
            ));
        }
    } else if (name == "reset") {
        if (instruction.operands.empty()) {
            circuit_.add_instruction(std::make_shared<Reset>(std::nullopt));
        } else {
            const auto &qubit_indices = operands_helper.get_register_operand(0);
            for (size_t i{ 0 }; i < qubit_indices.size(); ++i) {
                circuit_.add_instruction(std::make_shared<Reset>(
                    core::QubitIndex{ static_cast<std::size_t>(qubit_indices[i]->value) }
                ));
            }
        }
    } else {
        throw CircuitBuilderError{ fmt::format("unsupported instruction: '{}'", name) };
    }
}

template <std::size_t NumberOfQubitOperands>
void CircuitBuilder::visit_gate_instruction(
    core::matrix_t<NumberOfQubitOperands> matrix,
    cqasm_v3x_operands_indices_t<NumberOfQubitOperands> operands_indices) {
    static_assert(NumberOfQubitOperands > 0);

#ifndef NDEBUG
    // SGMQ check: all the operands use the same number of indices
    std::for_each(operands_indices.begin(), operands_indices.end(),
        [&operands_indices](CqasmV3xIndices const &indices) {
            assert(indices.size() == operands_indices[0].size());
        });
#endif

    // SGMQ unrolling: add one instruction for each operand's index
    for (std::size_t i = 0; i < operands_indices[0].size(); ++i) {
        core::operands_t<NumberOfQubitOperands> operands{};
        for (std::size_t qubit_index = 0; qubit_index < NumberOfQubitOperands; ++qubit_index) {
            operands[qubit_index] = core::QubitIndex{
                static_cast<std::size_t>(operands_indices[qubit_index][i]->value)
            };
        }
        circuit_.add_instruction(std::make_shared<Unitary<NumberOfQubitOperands>>(matrix, operands));
    }
}

}  // namespace qx
