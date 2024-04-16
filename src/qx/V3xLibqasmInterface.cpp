#include "qx/V3xLibqasmInterface.hpp"

#include "qx/Circuit.hpp"
#include "qx/Core.hpp"
#include "qx/Gates.hpp"
#include "v3x/cqasm-semantic-gen.hpp"

#include <algorithm>  // generate_n


namespace qx {

namespace v3cq = ::cqasm::v3x::semantic;
namespace v3primitives = ::cqasm::v3x::primitives;
namespace v3tree = ::cqasm::tree;
namespace v3types = ::cqasm::v3x::types;
namespace v3values = ::cqasm::v3x::values;

namespace {
class OperandsHelper {
public:
    explicit OperandsHelper(const v3cq::Instruction &instruction)
        : instruction(instruction) {}

    [[nodiscard]] v3cq::Many<v3values::ConstInt> get_register_operand(int id) const {
        if (auto variable_ref = instruction.operands[id]->as_variable_ref()) {
            auto variable_size = v3types::size_of(variable_ref->variable->typ);
            auto ret = v3cq::Many<v3values::ConstInt>{};
            ret.get_vec().resize(variable_size);
            std::generate_n(ret.get_vec().begin(), variable_size, [i=0]() mutable {
                return v3tree::make<v3values::ConstInt>(static_cast<v3primitives::Int>(i++));
            });
            return ret;
        }
        return instruction.operands[id]->as_index_ref()->indices;
    }

    [[nodiscard]] double get_float_operand(int id) const {
        return instruction.operands[id]->as_const_float()->value;
    }

    [[nodiscard]] std::int64_t get_int_operand(int id) const {
        return instruction.operands[id]->as_const_int()->value;
    }

private:
    const v3cq::Instruction &instruction;
};

class GateConvertor : public v3cq::RecursiveVisitor {
public:
    explicit GateConvertor(qx::Circuit &c) : circuit(c) {}

    void visit_instruction(v3cq::Instruction &instr) override { addGates(instr); }

    void visit_node(v3cq::Node &) override {
        throw std::runtime_error("Statement not supported by the simulator");
    }

private:
    template <std::size_t NumberOfQubitOperands>
    void addGates(
        core::DenseUnitaryMatrix<1 << NumberOfQubitOperands> matrix,
        std::array<v3cq::Many<v3values::ConstInt>, NumberOfQubitOperands> operands) {
        static_assert(NumberOfQubitOperands > 0);

#ifndef NDEBUG
        std::for_each(operands.begin(), operands.end(),
            [&operands](v3cq::Many<v3values::ConstInt> const &ops) {
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
            circuit.addInstruction(unitary, controlBits);
        }
    }

    void addGates(const v3cq::Instruction &instruction) {
        auto &name = instruction.instruction_ref->name;
        OperandsHelper operands(instruction);

        if (name == "TOFFOLI") {
            addGates<3>(gates::TOFFOLI, {
                operands.get_register_operand(0),
                operands.get_register_operand(1),
                operands.get_register_operand(2)
            });
        } else if (name == "I") {
            addGates<1>(gates::IDENTITY, {operands.get_register_operand(0)});
        } else if (name == "X") {
            addGates<1>(gates::X, {operands.get_register_operand(0)});
        } else if (name == "Y") {
            addGates<1>(gates::Y, {operands.get_register_operand(0)});
        } else if (name == "Z") {
            addGates<1>(gates::Z, {operands.get_register_operand(0)});
        } else if (name == "H") {
            addGates<1>(gates::H, {operands.get_register_operand(0)});
        } else if (name == "S") {
            addGates<1>(gates::S, {operands.get_register_operand(0)});
        } else if (name == "Sdag") {
            addGates<1>(gates::SDAG, {operands.get_register_operand(0)});
        } else if (name == "T") {
            addGates<1>(gates::T, {operands.get_register_operand(0)});
        } else if (name == "Tdag") {
            addGates<1>(gates::TDAG, {operands.get_register_operand(0)});
        } else if (name == "Rx") {
            addGates<1>(gates::RX(operands.get_float_operand(1)), { operands.get_register_operand(0) });
        } else if (name == "Ry") {
            addGates<1>(gates::RY(operands.get_float_operand(1)), { operands.get_register_operand(0) });
        } else if (name == "Rz") {
            addGates<1>(gates::RZ(operands.get_float_operand(1)), { operands.get_register_operand(0) });
        } else if (name == "CNOT") {
            addGates<2>(gates::CNOT, { operands.get_register_operand(0), operands.get_register_operand(1) });
        } else if (name == "CZ") {
            addGates<2>(gates::CZ, { operands.get_register_operand(0), operands.get_register_operand(1) });
        } else if (name == "SWAP") {
            addGates<2>(gates::SWAP, { operands.get_register_operand(0), operands.get_register_operand(1) });
        } else if (name == "X90") {
            return addGates<1>(gates::X90, { operands.get_register_operand(0) });
        } else if (name == "mX90") {
            return addGates<1>(gates::MX90, { operands.get_register_operand(0) });
        } else if (name == "Y90") {
            return addGates<1>(gates::Y90, { operands.get_register_operand(0) });
        } else if (name == "mY90") {
            return addGates<1>(gates::MY90, { operands.get_register_operand(0) });
        } else if (name == "measure") {
            for (const auto &q : operands.get_register_operand(0)) {
                auto controlBits = std::make_shared<std::vector<core::QubitIndex>>();
                circuit.addInstruction(
                    Circuit::Measure{ core::QubitIndex{ static_cast<std::size_t>(q->value) } },
                    controlBits
                );
            }
        } else if (name == "CR") {
            addGates<2>(gates::CR(operands.get_float_operand(2)),
                { operands.get_register_operand(0), operands.get_register_operand(1) });
        } else if (name == "CRk") {
            addGates<2>( gates::CR(static_cast<double>(gates::PI) / std::pow(2, operands.get_int_operand(2) - 1)),
                { operands.get_register_operand(0), operands.get_register_operand(1) });
        } else {
            throw std::runtime_error("Unsupported gate or instruction: " + name);
        }
    }

    qx::Circuit &circuit;
};
} // namespace

qx::Circuit loadCqasmCode(v3cq::Program const &program) {
    qx::Circuit circuit("cqasm 3.0 circuit", 1);

    for (const auto &statement : program.block->statements) { // program.global_block->statements
        GateConvertor gateConvertor(circuit);

        statement->visit(gateConvertor);
    }

    return circuit;
}

} // namespace qx
