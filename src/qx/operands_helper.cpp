#include "qx/operands_helper.hpp"

#include <algorithm>  // generate_n, transform
#include <cassert>

#include "qx/core.hpp"
#include "qx/cqasm_v3x.hpp"
#include "qx/register_manager.hpp"

namespace qx {

[[nodiscard]] InstructionsIndices get_instructions_indices(const CqasmV3xOperands& operands) {
    return to_instructions_indices(to_cqasm_v3x_instructions_indices(get_cqasm_v3x_sgmq_groups_indices(operands)));
}

[[nodiscard]] CqasmV3xSgmqGroupsIndices get_cqasm_v3x_sgmq_groups_indices(const CqasmV3xOperands& operands) {
    auto ret = CqasmV3xSgmqGroupsIndices(operands.size());
    std::transform(operands.begin(), operands.end(), ret.begin(), [](const auto& operand) {
        return get_cqasm_v3x_sgmq_group_indices(*operand);
    });
    return ret;
}

[[nodiscard]] CqasmV3xSgmqGroupIndices get_cqasm_v3x_sgmq_group_indices(const CqasmV3xOperand& operand) {
    const auto& register_manager = RegisterManager::get_instance();
    if (auto variable_ref = operand.as_variable_ref()) {
        auto ret = CqasmV3xSgmqGroupIndices{};
        if (is_qubit_variable(*variable_ref->variable)) {
            auto qubit_range = register_manager.get_qubit_range(variable_ref->variable->name);
            ret.get_vec().resize(qubit_range.size);
            std::generate_n(ret.get_vec().begin(), qubit_range.size, [qubit_range, i = 0]() mutable {
                return cqasm_tree::make<CqasmV3xConstInt>(
                    static_cast<cqasm_v3x_primitives::Int>(qubit_range.first + i++));
            });
        } else if (is_bit_variable(*variable_ref->variable)) {
            auto bit_range = register_manager.get_bit_range(variable_ref->variable->name);
            ret.get_vec().resize(bit_range.size);
            std::generate_n(ret.get_vec().begin(), bit_range.size, [bit_range, i = 0]() mutable {
                return cqasm_tree::make<CqasmV3xConstInt>(
                    static_cast<cqasm_v3x_primitives::Int>(bit_range.first + i++));
            });
        }
        return ret;
    } else if (auto index_ref = operand.as_index_ref()) {
        const auto& indices = index_ref->indices;
        auto ret = CqasmV3xSgmqGroupIndices{};
        ret.get_vec().resize(indices.size());
        if (is_qubit_variable(*index_ref->variable)) {
            auto qubit_range = register_manager.get_qubit_range(index_ref->variable->name);
            std::transform(indices.get_vec().begin(),
                indices.get_vec().end(),
                ret.get_vec().begin(),
                [qubit_range](const auto& index) {
                    return cqasm_tree::make<CqasmV3xConstInt>(
                        static_cast<cqasm_v3x_primitives::Int>(qubit_range.first + index->value));
                });
        } else if (is_bit_variable(*index_ref->variable)) {
            auto bit_range = register_manager.get_bit_range(index_ref->variable->name);
            std::transform(indices.get_vec().begin(),
                indices.get_vec().end(),
                ret.get_vec().begin(),
                [bit_range](const auto& index) {
                    return cqasm_tree::make<CqasmV3xConstInt>(
                        static_cast<cqasm_v3x_primitives::Int>(bit_range.first + index->value));
                });
        }
        return ret;
    } else {
        throw std::runtime_error{ "operand is neither a variable reference nor an index reference" };
    }
}

// Due to Single Gate Multiple Qubit (SGMQ), each operand of a cQASM v3x instruction can contain a list of indices
// Here, the term SGMQ group is used to refer to the list of indices of a cQASM v3x operand
// Now, after unrolling an SGMQ instruction, each operand of the unrolled instruction will contain a single index
// But the unrolled instruction, since it can have multiple operands, will contain a list of indices, one per operand
// This function converts a list of operands with a list of indices (SGMQ groups)
// into a list of instructions with a list of indices (instructions indices)
//
// For example, for CNOT q[0:2], q[3:5]
// The list of operands with a list of indices (SGMQ groups) are: [[0, 1, 2], [3, 4, 5]]
// After unrolling to: CNOT q[0], q[3]; CNOT q[1], q[4]; CNOT q[2], q[5]
// The list of instructions with a list of indices (instructions indices) will be: [[0, 3], [1, 4], [2, 5]]
[[nodiscard]] CqasmV3xInstructionsIndices to_cqasm_v3x_instructions_indices(
    const CqasmV3xSgmqGroupsIndices& sgmq_groups_indices) {
    if (sgmq_groups_indices.empty()) {
        return CqasmV3xInstructionsIndices{};
    }
    // The number of indices per SGMQ group determines the number of instructions
    auto ret = CqasmV3xInstructionsIndices{ sgmq_groups_indices[0].size() };
    for (const auto& sgmq_group_indices : sgmq_groups_indices) {
        for (size_t j{ 0 }; j < sgmq_group_indices.size(); ++j) {
            ret[j].add(sgmq_group_indices[j]);
        }
    }
    return ret;
}

[[nodiscard]] InstructionsIndices to_instructions_indices(const CqasmV3xInstructionsIndices& instructions_indices) {
    auto ret = InstructionsIndices(instructions_indices.size());
    std::transform(
        instructions_indices.begin(), instructions_indices.end(), ret.begin(), [](const auto& instruction_indices) {
            return to_instruction_indices(instruction_indices);
        });
    return ret;
}

[[nodiscard]] InstructionIndices to_instruction_indices(const CqasmV3xInstructionIndices& instruction_indices) {
    auto ret = InstructionIndices(instruction_indices.size());
    std::transform(
        instruction_indices.begin(), instruction_indices.end(), ret.begin(), [](const auto& instruction_index) {
            return core::QubitIndex{ static_cast<std::size_t>(instruction_index->value) };
        });
    return ret;
}

}  // namespace qx
