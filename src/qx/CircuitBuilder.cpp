#include "qx/CircuitBuilder.hpp"
#include "qx/Core.hpp"
#include "qx/Gates.hpp"
#include "qx/Instructions.hpp" // Measure, Unitary
#include "qx/OperandsHelper.hpp"

#include <algorithm>  // for_each
#include <fmt/format.h>
#include <memory>  // make_shared


namespace qx {

CircuitBuilder::CircuitBuilder(V3OneProgram const& program, RegisterManager const& register_manager)
    : program_{ program }
    , register_manager_{ register_manager }
    , circuit_{}
{}

Circuit CircuitBuilder::build() {
    std::for_each(program_->block->statements.begin(), program_->block->statements.end(),
        [this](auto const& statement) {
            statement->visit(*this);
        });
    return circuit_;
}

void CircuitBuilder::visit_node(V3Node &) {
    throw CircuitBuilderError{ "unsupported node" };
}

void CircuitBuilder::visit_instruction(V3Instruction &instruction) {
    auto &name = instruction.instruction_ref->name;
    auto operands_helper = OperandsHelper{ instruction, register_manager_ };
    
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
    matrix_t<NumberOfQubitOperands> matrix,
    ast_operands_t<NumberOfQubitOperands> operands) {
    static_assert(NumberOfQubitOperands > 0);

#ifndef NDEBUG
    std::for_each(operands.begin(), operands.end(),
        [&operands](V3Many<V3ConstInt> const &ops) {
            assert(ops.size() == operands[0].size());
        });
#endif

    for (std::size_t i = 0; i < operands[0].size(); ++i) {
        operands_t<NumberOfQubitOperands> ops{};
        for (std::size_t op = 0; op < NumberOfQubitOperands; ++op) {
            ops[op] = core::QubitIndex{ static_cast<std::size_t>(operands[op][i]->value) };
        }

        circuit_.add_instruction(std::make_shared<Unitary<NumberOfQubitOperands>>(matrix, ops));
    }
}

}  // namespace qx
