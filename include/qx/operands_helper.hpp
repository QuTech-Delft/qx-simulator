#pragma once

#include "qx/cqasm_v3x.hpp"

namespace qx {

[[nodiscard]] CqasmV3xIndices get_operand_indices(const CqasmV3xOperand& operand);

}  // namespace qx
