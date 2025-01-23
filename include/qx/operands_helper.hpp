#pragma once

#include "qx/cqasm_v3x.hpp"
#include "qx/core.hpp"

#include <vector>

namespace qx {

using InstructionIndices = std::vector<core::Index>;
using InstructionsIndices = std::vector<InstructionIndices>;

using CqasmV3xInstructionIndices = CqasmV3xOperandIndices;
using CqasmV3xInstructionsIndices = std::vector<CqasmV3xOperandIndices>;
using CqasmV3xSgmqGroupIndices = CqasmV3xOperandIndices;
using CqasmV3xSgmqGroupsIndices = std::vector<CqasmV3xSgmqGroupIndices>;

[[nodiscard]] InstructionsIndices get_instructions_indices(const CqasmV3xOperands& cqasm_v3x_instruction_indices);
[[nodiscard]] InstructionsIndices to_instructions_indices(const CqasmV3xInstructionsIndices& instructions_indices);
[[nodiscard]] InstructionIndices to_instruction_indices(const CqasmV3xInstructionIndices& instruction_indices);
[[nodiscard]] CqasmV3xInstructionsIndices to_cqasm_v3x_instructions_indices(const CqasmV3xSgmqGroupsIndices& sgmq_groups_indices);
[[nodiscard]] CqasmV3xSgmqGroupsIndices get_cqasm_v3x_sgmq_groups_indices(const CqasmV3xOperands& operands);
[[nodiscard]] CqasmV3xSgmqGroupIndices get_cqasm_v3x_sgmq_group_indices(const CqasmV3xOperand& operand);

}  // namespace qx
