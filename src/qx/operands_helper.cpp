#include "qx/operands_helper.hpp"

#include <algorithm>  // generate_n
#include <cassert>

#include "qx/cqasm_v3x.hpp"
#include "qx/register_manager.hpp"

namespace qx {

[[nodiscard]] CqasmV3xIndices get_operand_indices(const CqasmV3xOperand& operand) {
    const auto& register_manager = RegisterManager::get_instance();
    if (auto variable_ref = operand.as_variable_ref()) {
        auto ret = CqasmV3xIndices{};
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
        } else {
            return {};
        }
        return ret;
    } else if (auto index_ref = operand.as_index_ref()) {
        auto ret = index_ref->indices;
        if (is_qubit_variable(*index_ref->variable)) {
            auto qubit_range = register_manager.get_qubit_range(index_ref->variable->name);
            std::for_each(ret.get_vec().begin(), ret.get_vec().end(), [qubit_range](const auto& index) {
                index->value += qubit_range.first;
            });
        } else if (is_bit_variable(*index_ref->variable)) {
            auto bit_range = register_manager.get_bit_range(index_ref->variable->name);
            std::for_each(ret.get_vec().begin(), ret.get_vec().end(), [bit_range](const auto& index) {
                index->value += bit_range.first;
            });
        } else {
            return {};
        }
        return ret;
    }
    assert(false && "operand is neither a variable reference nor an index reference");
    return {};
}

}  // namespace qx
