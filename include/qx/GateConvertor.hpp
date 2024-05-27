#pragma once

#include "qx/Circuit.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SimulationError.hpp"
#include "qx/V3xLibqasmInterface.hpp"

#include <algorithm>  // for_each
#include <string>


namespace qx {

/*
 * GateConvertorError
 */
struct GateConvertorError : public SimulationError {
    explicit GateConvertorError(const std::string &message) : SimulationError{ message } {}
};

/*
 * GateConvertor
 */
class GateConvertor : public V3RecursiveVisitor {
public:
    explicit GateConvertor(Circuit &c);
    void visit_instruction(V3Instruction &instr) override;
    void visit_node(V3Node &) override;

private:
    template <std::size_t NumberOfQubitOperands>
    void addGates(
        core::DenseUnitaryMatrix<1 << NumberOfQubitOperands> matrix,
        std::array<V3Many<V3ConstInt>, NumberOfQubitOperands> operands);
    void addGates(const V3Instruction &instruction);

private:
    Circuit &circuit_;
};

}  // namespace qx
