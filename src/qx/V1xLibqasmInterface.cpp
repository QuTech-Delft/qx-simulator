#include "qx/V1xLibqasmInterface.hpp"

#include "qx/Circuit.hpp"
#include "qx/Core.hpp"
#include "qx/Gates.hpp"
#include "v1x/cqasm-semantic-gen.hpp"


namespace qx {

namespace v1cq = ::cqasm::v1x::semantic;
namespace v1values = ::cqasm::v1x::values;


namespace {
class OperandsHelper {
public:
    explicit OperandsHelper(const v1cq::Instruction &instruction)
        : instruction(instruction) {}

    [[nodiscard]] v1cq::Many<v1values::ConstInt> get_qubit_operands(int id) const {
        return instruction.operands[id]->as_qubit_refs()->index;
    }

    [[nodiscard]] v1cq::Many<v1values::ConstInt> get_bit_operands(int id) const {
        return instruction.operands[id]->as_bit_refs()->index;
    }

    [[nodiscard]] double get_float_operand(int id) const {
        return instruction.operands[id]->as_const_real()->value;
    }

    [[nodiscard]] std::int64_t get_int_operand(int id) const {
        return instruction.operands[id]->as_const_int()->value;
    }

private:
    const v1cq::Instruction &instruction;
};

std::string to_string(v1cq::NodeType nodeType) {
    switch (nodeType) {
    case v1cq::NodeType::AnnotationData:
        return "AnnotationData";
    case v1cq::NodeType::Block:
        return "Block";
    case v1cq::NodeType::BreakStatement:
        return "BreakStatement";
    case v1cq::NodeType::Bundle:
        return "Bundle";
    case v1cq::NodeType::BundleExt:
        return "BundleExt";
    case v1cq::NodeType::ContinueStatement:
        return "ContinueStatement";
    case v1cq::NodeType::ErrorModel:
        return "ErrorModel";
    case v1cq::NodeType::ForLoop:
        return "ForLoop";
    case v1cq::NodeType::ForeachLoop:
        return "ForeachLoop";
    case v1cq::NodeType::GotoInstruction:
        return "GotoInstruction";
    case v1cq::NodeType::IfElse:
        return "IfElse";
    case v1cq::NodeType::IfElseBranch:
        return "IfElseBranch";
    case v1cq::NodeType::Instruction:
        return "Instruction";
    case v1cq::NodeType::Mapping:
        return "Mapping";
    case v1cq::NodeType::Program:
        return "Program";
    case v1cq::NodeType::RepeatUntilLoop:
        return "RepeatUntilLoop";
    case v1cq::NodeType::SetInstruction:
        return "SetInstruction";
    case v1cq::NodeType::Subcircuit:
        return "Subcircuit";
    case v1cq::NodeType::Variable:
        return "Variable";
    case v1cq::NodeType::Version:
        return "Version";
    case v1cq::NodeType::WhileLoop:
        return "WhileLoop";
    }

    return "Unknown";
}

class GateConvertor : public v1cq::RecursiveVisitor {
public:
    explicit GateConvertor(qx::Circuit &c) : circuit(c) {}

    void visit_instruction(v1cq::Instruction &instr) override { addGates(instr); }

    void visit_bundle_ext(v1cq::BundleExt &node) override {
        node.items.visit(*this);
    }

    void visit_node(v1cq::Node &node) override {
        throw std::runtime_error("Statements of the following type are not supported by the simulator: " +
                                 to_string(node.type()));
    }

private:
    void addGates(const v1cq::Instruction &instruction) {
        auto b = instruction.condition->as_const_bool();
        if (b && !(b->value)) {
            return;
        }

        std::shared_ptr<std::vector<core::QubitIndex>> controlBits;

        if (auto bit_ref = instruction.condition->as_bit_refs()) {
            controlBits = std::make_shared<std::vector<core::QubitIndex>>();
            for (auto const &index : bit_ref->index) {
                controlBits->push_back(
                    core::QubitIndex{static_cast<std::size_t>(index->value)});
            }
        }

        addGates(instruction, controlBits);
    }

    template <std::size_t NumberOfQubitOperands>
    void addGates(
        core::DenseUnitaryMatrix<1 << NumberOfQubitOperands> matrix,
        std::array<v1cq::Many<v1values::ConstInt>, NumberOfQubitOperands> operands,
        const std::shared_ptr<std::vector<core::QubitIndex>> &controlBits) {
        static_assert(NumberOfQubitOperands > 0);

#ifndef NDEBUG
        std::for_each(operands.begin(), operands.end(),
                      [&operands](v1cq::Many<v1values::ConstInt> const &ops) {
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
            circuit.addInstruction(unitary, controlBits);
        }
    }

    void addGates(const v1cq::Instruction &instruction,
                  const std::shared_ptr<std::vector<core::QubitIndex>> &controlBits) {
        auto &name = instruction.instruction->name;
        OperandsHelper operands(instruction);

        if (name == "toffoli") {
            addGates<3>(gates::TOFFOLI,
                        {operands.get_qubit_operands(0),
                         operands.get_qubit_operands(1),
                         operands.get_qubit_operands(2)},
                        controlBits);
        } else if (name == "i") {
            addGates<1>(gates::IDENTITY, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "x") {
            addGates<1>(gates::X, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "y") {
            addGates<1>(gates::Y, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "z") {
            addGates<1>(gates::Z, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "h") {
            addGates<1>(gates::H, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "s") {
            addGates<1>(gates::S, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "sdag") {
            addGates<1>(gates::SDAG, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "t") {
            addGates<1>(gates::T, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "tdag") {
            addGates<1>(gates::TDAG, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "not") {
            BasisVector mask{};
            for (const auto &b : operands.get_bit_operands(0)) {
                mask.set(b->value);
            }

            circuit.addInstruction(
                Circuit::MeasurementRegisterOperation{
                    [mask](auto &bitReg) { bitReg ^= mask; }},
                controlBits);
        } else if (name == "rx") {
            addGates<1>(gates::RX(operands.get_float_operand(1)),
                        {operands.get_qubit_operands(0)}, controlBits);
        } else if (name == "ry") {
            addGates<1>(gates::RY(operands.get_float_operand(1)),
                        {operands.get_qubit_operands(0)}, controlBits);
        } else if (name == "rz") {
            addGates<1>(gates::RZ(operands.get_float_operand(1)),
                        {operands.get_qubit_operands(0)}, controlBits);
        } else if (name == "cnot") {
            addGates<2>(gates::CNOT,
                        {operands.get_qubit_operands(0),
                         operands.get_qubit_operands(1)},
                        controlBits);
        } else if (name == "cz") {
            addGates<2>(gates::CZ,
                        {operands.get_qubit_operands(0),
                         operands.get_qubit_operands(1)},
                        controlBits);
        } else if (name == "swap") {
            addGates<2>(gates::SWAP,
                        {operands.get_qubit_operands(0),
                         operands.get_qubit_operands(1)},
                        controlBits);
        } else if (name == "prep_x") {
            for (const auto &q : operands.get_qubit_operands(0)) {
                circuit.addInstruction(Circuit::PrepZ{core::QubitIndex{
                                           static_cast<std::size_t>(q->value)}},
                                       controlBits);
            }
            addGates<1>(gates::H, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "prep_y") {
            for (const auto &q : operands.get_qubit_operands(0)) {
                circuit.addInstruction(Circuit::PrepZ{core::QubitIndex{
                                           static_cast<std::size_t>(q->value)}},
                                       controlBits);
            }
            addGates<1>(gates::H, {operands.get_qubit_operands(0)},
                        controlBits);
            addGates<1>(gates::S, {operands.get_qubit_operands(0)},
                        controlBits);
        } else if (name == "prep_z") {
            for (const auto &q : operands.get_qubit_operands(0)) {
                circuit.addInstruction(Circuit::PrepZ{core::QubitIndex{
                                           static_cast<std::size_t>(q->value)}},
                                       controlBits);
            }
        } else if (name == "measure" || name == "measure_z") {
            for (const auto &q : operands.get_qubit_operands(0)) {
                circuit.addInstruction(Circuit::Measure{core::QubitIndex{
                                           static_cast<std::size_t>(q->value)}},
                                       controlBits);
            }
        } else if (name == "measure_all") {
            circuit.addInstruction(Circuit::MeasureAll(), controlBits);
        } else if (name == "measure_x") {
            addGates<1>(gates::H, {operands.get_qubit_operands(0)},
                        controlBits);
            for (const auto &q : operands.get_qubit_operands(0)) {
                circuit.addInstruction(Circuit::Measure{core::QubitIndex{
                                           static_cast<std::size_t>(q->value)}},
                                       controlBits);
            }
        } else if (name == "measure_y") {
            addGates<1>(gates::SDAG, {operands.get_qubit_operands(0)},
                        controlBits);
            addGates<1>(gates::H, {operands.get_qubit_operands(0)},
                        controlBits);
            for (const auto &q : operands.get_qubit_operands(0)) {
                circuit.addInstruction(Circuit::Measure{core::QubitIndex{
                                           static_cast<std::size_t>(q->value)}},
                                       controlBits);
            }
        } else if (name == "x90") {
            return addGates<1>(gates::X90, {operands.get_qubit_operands(0)},
                               controlBits);
        } else if (name == "mx90") {
            return addGates<1>(gates::MX90, {operands.get_qubit_operands(0)},
                               controlBits);
        } else if (name == "y90") {
            return addGates<1>(gates::Y90, {operands.get_qubit_operands(0)},
                               controlBits);
        } else if (name == "my90") {
            return addGates<1>(gates::MY90, {operands.get_qubit_operands(0)},
                               controlBits);
        } else if (name == "cr") {
            addGates<2>(gates::CR(operands.get_float_operand(2)),
                        {operands.get_qubit_operands(0),
                         operands.get_qubit_operands(1)},
                        controlBits);
        } else if (name == "crk") {
            addGates<2>(gates::CR(static_cast<double>(gates::PI) /
                                  std::pow(2, operands.get_int_operand(2) - 1)),
                        {operands.get_qubit_operands(0),
                         operands.get_qubit_operands(1)},
                        controlBits);
        } else {
            throw std::runtime_error("Unsupported gate: " + name);
        }
    }

    qx::Circuit &circuit;
};
} // namespace

qx::Circuit loadCqasmCode(v1cq::Subcircuit const &subcircuit) {
    qx::Circuit circuit(subcircuit.name, subcircuit.iterations);

    for (const auto &statement : subcircuit.body->statements) {
        GateConvertor gateConvertor(circuit);

        statement->visit(gateConvertor);
    }

    return circuit;
}

} // namespace qx
