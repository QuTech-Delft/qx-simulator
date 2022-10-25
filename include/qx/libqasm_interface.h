#pragma once

#include "qx/compat.h"
#include "qx/core/circuit.h"
#include "cqasm-v1-semantic-gen.hpp"

namespace cq = ::cqasm::v1::semantic;
namespace values = ::cqasm::v1::values;

class OperandsHelper {
public:
    OperandsHelper(const cq::Instruction &instruction) : instruction(instruction) {}

    cq::Many<values::ConstInt> get_qubit_operands(int id) const {
        return instruction.operands[id]->as_qubit_refs()->index;
    }

    cq::Many<values::ConstInt> get_bit_operands(int id) const {
        return instruction.operands[id]->as_bit_refs()->index;
    }

    double get_float_operand(int id) const {
        return instruction.operands[id]->as_const_real()->value;
    }

    std::int64_t get_int_operand(int id) const {
        return instruction.operands[id]->as_const_int()->value;
    }

private:
    const cq::Instruction &instruction;
};

template <typename GateT>
std::vector<std::shared_ptr<qx::gate>> make_gates(cq::Many<values::ConstInt> qubits) {
    std::vector<std::shared_ptr<qx::gate>> gates{};

    for (const auto& qubit: qubits) {
        gates.push_back(std::make_shared<GateT>(qubit->value));
    }

    return gates;
}

template <typename GateT, typename Arg>
std::vector<std::shared_ptr<qx::gate>> make_gates(cq::Many<values::ConstInt> qubits, Arg&& arg) {
    std::vector<std::shared_ptr<qx::gate>> gates{};

    for (const auto& qubit: qubits) {
        gates.push_back(std::make_shared<GateT>(qubit->value, arg));
    }

    return gates;
}

template <typename GateT>
std::vector<std::shared_ptr<qx::gate>> make_gates(cq::Many<values::ConstInt> qubits1, cq::Many<values::ConstInt> qubits2) {
    assert(qubits1.size() == qubits2.size());

    std::vector<std::shared_ptr<qx::gate>> gates{};

    for (std::size_t i = 0; i < qubits1.size(); ++i) {
        gates.push_back(std::make_shared<GateT>(qubits1[i]->value, qubits2[i]->value));
    }

    return gates;
}

template <typename GateT, typename Arg>
std::vector<std::shared_ptr<qx::gate>> make_gates(cq::Many<values::ConstInt> qubits1, cq::Many<values::ConstInt> qubits2, Arg&& arg) {
    assert(qubits1.size() == qubits2.size());

    std::vector<std::shared_ptr<qx::gate>> gates{};

    for (std::size_t i = 0; i < qubits1.size(); ++i) {
        gates.push_back(std::make_shared<GateT>(qubits1[i]->value, qubits2[i]->value, arg));
    }

    return gates;
}

template <typename GateT>
std::vector<std::shared_ptr<qx::gate>> make_gates(cq::Many<values::ConstInt> qubits1, cq::Many<values::ConstInt> qubits2, cq::Many<values::ConstInt> qubits3) {
    assert(qubits1.size() == qubits2.size() && qubits2.size() == qubits3.size());

    std::vector<std::shared_ptr<qx::gate>> gates{};

    for (std::size_t i = 0; i < qubits1.size(); ++i) {
        gates.push_back(std::make_shared<GateT>(qubits1[i]->value, qubits2[i]->value, qubits3[i]->value));
    }

    return gates;
}

std::vector<std::shared_ptr<qx::gate>> gateLookupWithoutCondition(const cq::Instruction &instruction) {
    auto &name = instruction.instruction->name;
    OperandsHelper operands(instruction);

    if (name == "toffoli") {
        return make_gates<qx::toffoli>(operands.get_qubit_operands(0), operands.get_qubit_operands(1), operands.get_qubit_operands(2));
    }

    if (name == "i") {
        return make_gates<qx::identity>(operands.get_qubit_operands(0));
    }

    if (name == "x") {
        return make_gates<qx::pauli_x>(operands.get_qubit_operands(0));
    }

    if (name == "y") {
        return make_gates<qx::pauli_y>(operands.get_qubit_operands(0));
    }

    if (name == "z") {
        return make_gates<qx::pauli_z>(operands.get_qubit_operands(0));
    }

    if (name == "h") {
        return make_gates<qx::hadamard>(operands.get_qubit_operands(0));
    }

    if (name == "s") {
        return make_gates<qx::phase_shift>(operands.get_qubit_operands(0));
    }

    if (name == "sdag") {
        return make_gates<qx::s_dag_gate>(operands.get_qubit_operands(0));
    }

    if (name == "t") {
        return make_gates<qx::t_gate>(operands.get_qubit_operands(0));
    }

    if (name == "tdag") {
        return make_gates<qx::t_dag_gate>(operands.get_qubit_operands(0));
    }

    if (name == "not") {
        return make_gates<qx::classical_not>(operands.get_bit_operands(0));
    }

    if (name == "rx") {
        return make_gates<qx::rx>(operands.get_qubit_operands(0), operands.get_float_operand(1));
    }

    if (name == "ry") {
        return make_gates<qx::ry>(operands.get_qubit_operands(0), operands.get_float_operand(1));
    }

    if (name == "rz") {
        return make_gates<qx::rz>(operands.get_qubit_operands(0), operands.get_float_operand(1));
    }

    if (name == "cnot") {
        return make_gates<qx::cnot>(operands.get_qubit_operands(0), operands.get_qubit_operands(1));
    }

    if (name == "cz") {
        return make_gates<qx::cphase>(operands.get_qubit_operands(0), operands.get_qubit_operands(1));
    }

    if (name == "swap") {
        return make_gates<qx::swap>(operands.get_qubit_operands(0), operands.get_qubit_operands(1));
    }

    if (name == "prep_x") {
        return make_gates<qx::prepx>(operands.get_qubit_operands(0));
    }

    if (name == "prep_y") {
        return make_gates<qx::prepy>(operands.get_qubit_operands(0));
    }

    if (name == "prep_z") {
        return make_gates<qx::prepz>(operands.get_qubit_operands(0));
    }

    if (name == "measure" || name == "measure_z") {
        return make_gates<qx::measure>(operands.get_qubit_operands(0));
    }

    if (name == "measure_all") {
        return {std::make_shared<qx::measure_all>()};
    }

    if (name == "measure_x") {
        return make_gates<qx::measure_x>(operands.get_qubit_operands(0));
    }

    if (name == "measure_y") {
        return make_gates<qx::measure_y>(operands.get_qubit_operands(0));
    }

    if (name == "display") {
        return {std::make_shared<qx::display>()};
    }

    if (name == "display_binary") {
        return {std::make_shared<qx::display>(true)};
    }

    if (name == "x90") {
        double angle = QX_PI / 2;
        return make_gates<qx::rx>(operands.get_qubit_operands(0), angle);
    }

    if (name == "mx90") {
        double angle = -QX_PI / 2;
        return make_gates<qx::rx>(operands.get_qubit_operands(0), angle);
    }

    if (name == "y90") {
        double angle = QX_PI / 2;
        return make_gates<qx::ry>(operands.get_qubit_operands(0), angle);
    }

    if (name == "my90") {
        double angle = -QX_PI / 2;
        return make_gates<qx::ry>(operands.get_qubit_operands(0), angle);
    }

    if (name == "cr") {
        return make_gates<qx::ctrl_phase_shift>(operands.get_qubit_operands(0), operands.get_qubit_operands(1), operands.get_float_operand(2));
    }

    if (name == "crk") {
        return make_gates<qx::ctrl_phase_shift>(operands.get_qubit_operands(0), operands.get_qubit_operands(1), static_cast<std::size_t>(operands.get_int_operand(2)));
    }

    return {};
}

std::vector<std::shared_ptr<qx::gate>> gateLookup(const cq::Instruction &instruction) {
    auto b = instruction.condition->as_const_bool();
    if (b && !(b->value)) {
        return {};
    }

    auto gates = gateLookupWithoutCondition(instruction);

    if (b && b->value) {
        return gates;
    }

    if (auto bitref = instruction.condition->as_bit_refs()) {
        std::vector<std::size_t> control_bits;
        for (auto const& b: bitref->index) {
            control_bits.push_back(b->value);
        }

        std::vector<std::shared_ptr<qx::gate>> controlled_gates{};

        for (auto const& gate: gates) {
            controlled_gates.push_back(std::make_shared<qx::bin_ctrl>(control_bits, gate));
        }

        return controlled_gates;
    }

    return {};
}

std::string to_string(cq::NodeType nodeType) {
    switch (nodeType) {
        case cq::NodeType::AnnotationData:
            return "AnnotationData";
        case cq::NodeType::Block:
            return "Block";
        case cq::NodeType::BreakStatement:
            return "BreakStatement";
        case cq::NodeType::Bundle:
            return "Bundle";
        case cq::NodeType::BundleExt:
            return "BundleExt";
        case cq::NodeType::ContinueStatement:
            return "ContinueStatement";
        case cq::NodeType::ErrorModel:
            return "ErrorModel";
        case cq::NodeType::ForLoop:
            return "ForLoop";
        case cq::NodeType::ForeachLoop:
            return "ForeachLoop";
        case cq::NodeType::GotoInstruction:
            return "GotoInstruction";
        case cq::NodeType::IfElse:
            return "IfElse";
        case cq::NodeType::IfElseBranch:
            return "IfElseBranch";
        case cq::NodeType::Instruction:
            return "Instruction";
        case cq::NodeType::Mapping:
            return "Mapping";
        case cq::NodeType::Program:
            return "Program";
        case cq::NodeType::RepeatUntilLoop:
            return "RepeatUntilLoop";
        case cq::NodeType::SetInstruction:
            return "SetInstruction";
        case cq::NodeType::Subcircuit:
            return "Subcircuit";
        case cq::NodeType::Variable:
            return "Variable";
        case cq::NodeType::Version:
            return "Version";
        case cq::NodeType::WhileLoop:
            return "WhileLoop";
    }

    return "Unknown";
}

class GateConvertor : public cq::RecursiveVisitor {
    public:
    GateConvertor(std::vector<std::shared_ptr<qx::gate>> &gs) : gates(gs) {}

    void visit_instruction(cq::Instruction &instr) override {
        const auto newGates = gateLookup(instr);
        for (const auto newGate: newGates) {
            gates.push_back(newGate);
        }
    }

    void visit_bundle_ext(cq::BundleExt &node) override {
        node.items.visit(*this);
    }

    void visit_node(cq::Node &node) override {
        throw std::runtime_error("Statements of the following type are not supported: " + to_string(node.type()));
    }

    private:
    std::vector<std::shared_ptr<qx::gate>> &gates;
};

std::shared_ptr<qx::circuit> load_cqasm_code(uint64_t qubits_count,
                                             cq::Subcircuit const &subcircuit) {
    auto iterations = subcircuit.iterations;
    auto name = subcircuit.name;

    auto circuit =
        std::make_shared<qx::circuit>(qubits_count, name, iterations);

    if (subcircuit.body.empty()) {
        return circuit;
    }

    for (const auto& statement : subcircuit.body->statements) {
        std::vector<std::shared_ptr<qx::gate>> gs{};

        GateConvertor gateConvertor(gs);

        statement->visit(gateConvertor);

        for (const auto &g : gs) {
            circuit->add(g); 
        }
    }
    return circuit;
}