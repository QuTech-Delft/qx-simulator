#pragma once

#include "qx/Circuit.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SimulationError.hpp"
#include "qx/V3xLibqasmInterface.hpp"

#include <algorithm>  // for_each
#include <string>


namespace qx {

/*
 * CircuitBuilderError
 */
struct CircuitBuilderError : public SimulationError {
    explicit CircuitBuilderError(const std::string &message) : SimulationError{ message } {}
};

/*
 * CircuitBuilder
 */
class CircuitBuilder : public V3RecursiveVisitor {
public:
    explicit CircuitBuilder(Circuit &circuit);
    Circuit build();

private:
    template <std::size_t N>
    using ast_operands_t = std::array<V3Many<V3ConstInt>, N>;

private:
    void visit_node(V3Node &) override;
    void visit_instruction(V3Instruction &instr) override;
    template <std::size_t NumberOfQubitOperands>
    void visit_gate_instruction(
        matrix_t<NumberOfQubitOperands> matrix,
        ast_operands_t<NumberOfQubitOperands> operands);

private:
    Circuit &circuit_;
};

}  // namespace qx
