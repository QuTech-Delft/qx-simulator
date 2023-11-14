#include "qx/V3xLibqasmInterface.hpp"

#include "qx/Circuit.hpp"
#include "qx/Core.hpp"
#include "qx/Gates.hpp"
#include "v3x/cqasm-semantic-gen.hpp"


namespace qx {

namespace v3cq = ::cqasm::v3x::semantic;
namespace v3values = ::cqasm::v3x::values;

namespace {
class OperandsHelper {
public:
    explicit OperandsHelper(const v3cq::Instruction &instruction)
        : instruction(instruction) {}

    [[nodiscard]] v3cq::Many<v3values::ConstInt> get_register_operand(int id) const {
        return instruction.operands[id]->as_index_ref()->indices;
    }

    [[nodiscard]] double get_float_operand(int id) const {
        return instruction.operands[id]->as_const_real()->value;
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
        auto &name = instruction.instruction->name;
        OperandsHelper operands(instruction);

        if (name == "toffoli") {
            addGates<3>(gates::TOFFOLI,
                        {operands.get_register_operand(0),
                         operands.get_register_operand(1),
                         operands.get_register_operand(2)});
        } else if (name == "i") {
            addGates<1>(gates::IDENTITY, {operands.get_register_operand(0)});
        } else if (name == "x") {
            addGates<1>(gates::X, {operands.get_register_operand(0)});
        } else if (name == "y") {
            addGates<1>(gates::Y, {operands.get_register_operand(0)});
        } else if (name == "z") {
            addGates<1>(gates::Z, {operands.get_register_operand(0)});
        } else if (name == "h") {
            addGates<1>(gates::H, {operands.get_register_operand(0)});
        } else if (name == "s") {
            addGates<1>(gates::S, {operands.get_register_operand(0)});
        } else if (name == "sdag") {
            addGates<1>(gates::SDAG, {operands.get_register_operand(0)});
        } else if (name == "t") {
            addGates<1>(gates::T, {operands.get_register_operand(0)});
        } else if (name == "tdag") {
            addGates<1>(gates::TDAG, {operands.get_register_operand(0)});
        } else if (name == "rx") {
            addGates<1>(gates::RX(operands.get_float_operand(1)),
                        {operands.get_register_operand(0)});
        } else if (name == "ry") {
            addGates<1>(gates::RY(operands.get_float_operand(1)),
                        {operands.get_register_operand(0)});
        } else if (name == "rz") {
            addGates<1>(gates::RZ(operands.get_float_operand(1)),
                        {operands.get_register_operand(0)});
        } else if (name == "cnot") {
            addGates<2>(gates::CNOT,
                        {operands.get_register_operand(0),
                         operands.get_register_operand(1)});
        } else if (name == "cz") {
            addGates<2>(gates::CZ,
                        {operands.get_register_operand(0),
                         operands.get_register_operand(1)});
        } else if (name == "swap") {
            addGates<2>(gates::SWAP,
                        {operands.get_register_operand(0),
                         operands.get_register_operand(1)});
        } else if (name == "x90") {
            return addGates<1>(gates::X90, {operands.get_register_operand(0)});
        } else if (name == "mx90") {
            return addGates<1>(gates::MX90, {operands.get_register_operand(0)});
        } else if (name == "y90") {
            return addGates<1>(gates::Y90, {operands.get_register_operand(0)});
        } else if (name == "my90") {
            return addGates<1>(gates::MY90, {operands.get_register_operand(0)});
        } else if (name == "measure") {
            // Classical bits == lhs == argument #0
            // Qubits == rhs == argument #1
            // FIXME: add assert that bit index == qubit index, otherwise not supported.
            for (const auto &q : operands.get_register_operand(1)) {
                auto controlBits = std::make_shared<std::vector<core::QubitIndex>>();
                circuit.addInstruction(Circuit::Measure{core::QubitIndex{
                                           static_cast<std::size_t>(q->value)}},
                                       controlBits);
            }
        } else if (name == "cr") {
            addGates<2>(gates::CR(operands.get_float_operand(2)),
                        {operands.get_register_operand(0),
                         operands.get_register_operand(1)});
        } else if (name == "crk") {
            addGates<2>(gates::CR(static_cast<double>(gates::PI) /
                                  std::pow(2, operands.get_int_operand(2) - 1)),
                        {operands.get_register_operand(0),
                         operands.get_register_operand(1)});
        } else {
            throw std::runtime_error("Unsupported gate or instruction: " + name);
        }
    }

    qx::Circuit &circuit;
};
} // namespace

qx::Circuit loadCqasmCode(v3cq::Program const &program) {
    qx::Circuit circuit("cqasm 3.0 circuit", 1);

    for (const auto &statement : program.statements) { // program.global_block->statements
        GateConvertor gateConvertor(circuit);

        statement->visit(gateConvertor);
    }

    return circuit;
}

} // namespace qx
