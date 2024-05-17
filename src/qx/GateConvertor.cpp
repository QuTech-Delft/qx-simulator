#include "qx/Core.hpp"
#include "qx/GateConvertor.hpp"
#include "qx/Gates.hpp"
#include "qx/OperandsHelper.hpp"

#include <fmt/format.h>


namespace qx {

GateConvertor::GateConvertor(Circuit &circuit)
    : circuit_{ circuit }
{}

void GateConvertor::visit_instruction(V3Instruction &instruction) {
    addGates(instruction);
}

void GateConvertor::visit_node(V3Node &) {
    throw GateConvertorError{ "Statement not supported by the simulator" };
}

template <std::size_t NumberOfQubitOperands>
void GateConvertor::addGates(
    core::DenseUnitaryMatrix<1 << NumberOfQubitOperands> matrix,
    std::array<V3Many<V3ConstInt>, NumberOfQubitOperands> operands) {
    static_assert(NumberOfQubitOperands > 0);

#ifndef NDEBUG
    std::for_each(operands.begin(), operands.end(),
        [&operands](V3Many<V3ConstInt> const &ops) {
            assert(ops.size() == operands[0].size());
        });
#endif

    for (std::size_t i = 0; i < operands[0].size(); ++i) {
        std::array<core::QubitIndex, NumberOfQubitOperands> ops{};
        for (std::size_t op = 0; op < NumberOfQubitOperands; ++op) {
            ops[op] = core::QubitIndex{
                static_cast<std::size_t>(operands[op][i]->value)};
        }

        Circuit::Unitary<NumberOfQubitOperands> unitary{matrix, ops};

        auto controlBits = std::make_shared<std::vector<core::QubitIndex>>();
        circuit_.add_instruction(unitary, controlBits);
    }
}

void GateConvertor::addGates(const V3Instruction &instruction) {
    auto &name = instruction.instruction_ref->name;
    auto operands_helper = OperandsHelper{ instruction, circuit_.get_register_manager() };

    if (name == "TOFFOLI") {
        addGates<3>(
            gates::TOFFOLI,
            {
                operands_helper.get_register_operand(0),
                operands_helper.get_register_operand(1),
                operands_helper.get_register_operand(2)
        });
    } else if (name == "I") {
        addGates<1>(gates::IDENTITY, { operands_helper.get_register_operand(0) });
    } else if (name == "X") {
        addGates<1>(gates::X, { operands_helper.get_register_operand(0) });
    } else if (name == "Y") {
        addGates<1>(gates::Y, { operands_helper.get_register_operand(0) });
    } else if (name == "Z") {
        addGates<1>(gates::Z, { operands_helper.get_register_operand(0) });
    } else if (name == "H") {
        addGates<1>(gates::H, { operands_helper.get_register_operand(0) });
    } else if (name == "S") {
        addGates<1>(gates::S, { operands_helper.get_register_operand(0) });
    } else if (name == "Sdag") {
        addGates<1>(gates::SDAG, { operands_helper.get_register_operand(0) });
    } else if (name == "T") {
        addGates<1>(gates::T, { operands_helper.get_register_operand(0) });
    } else if (name == "Tdag") {
        addGates<1>(gates::TDAG, {operands_helper.get_register_operand(0) });
    } else if (name == "Rx") {
        addGates<1>(
            gates::RX(operands_helper.get_float_operand(1)),
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "Ry") {
        addGates<1>(
            gates::RY(operands_helper.get_float_operand(1)),
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "Rz") {
        addGates<1>(
            gates::RZ(operands_helper.get_float_operand(1)),
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "CNOT") {
        addGates<2>(
            gates::CNOT,
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "CZ") {
        addGates<2>(
            gates::CZ,
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "SWAP") {
        addGates<2>(
            gates::SWAP,
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "X90") {
        return addGates<1>(
            gates::X90,
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "mX90") {
        return addGates<1>(
            gates::MX90,
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "Y90") {
        return addGates<1>(
            gates::Y90,
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "mY90") {
        return addGates<1>(
            gates::MY90,
            { operands_helper.get_register_operand(0) }
        );
    } else if (name == "measure") {
        // A measure statement has the following syntax: b = measure q
        // The left-hand side operand, b, is the operand 0
        // The right-hand side operand, q, is the operand 1
        for (const auto &q: operands_helper.get_register_operand(1)) {
            auto controlBits = std::make_shared<std::vector<core::QubitIndex>>();
            circuit_.add_instruction(Circuit::Measure{ core::QubitIndex{ static_cast<std::size_t>(q->value) } },
                                     controlBits);
        }
    } else if (name == "CR") {
        addGates<2>(
            gates::CR(operands_helper.get_float_operand(2)),
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else if (name == "CRk") {
        addGates<2>(
            gates::CR(static_cast<double>(gates::PI) / std::pow(2, operands_helper.get_int_operand(2) - 1)),
            { operands_helper.get_register_operand(0), operands_helper.get_register_operand(1) }
        );
    } else {
        throw GateConvertorError{ fmt::format("Unsupported gate or instruction: {}", name) };
    }
}

}  // namespace qx
