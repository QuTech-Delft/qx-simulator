#include "qx/operands_helper.hpp"
#include "qx/cqasm_v3x.hpp"
#include "qx/register_manager.hpp"

#include <algorithm>  // generate_n
#include <cassert>


namespace qx {

OperandsHelper::OperandsHelper(const CqasmV3xInstruction &instruction, const RegisterManager &register_manager)
    : instruction_{ instruction }
    , register_manager_{ register_manager }
{}

[[nodiscard]] CqasmV3xIndices OperandsHelper::get_register_operand(int id) const {
    if (auto variable_ref = instruction_.operands[id]->as_variable_ref()) {
        auto ret = CqasmV3xIndices{};
        if (is_qubit_variable(*variable_ref->variable)) {
            auto qubit_range = register_manager_.get_qubit_range(variable_ref->variable->name);
            ret.get_vec().resize(qubit_range.size);
            std::generate_n(ret.get_vec().begin(), qubit_range.size, [qubit_range, i=0]() mutable {
                return cqasm_v3x_tree::make<CqasmV3xConstInt>(
                    static_cast<cqasm_v3x_primitives::Int>(qubit_range.first + i++)
                );
            });
        } else if (is_bit_variable(*variable_ref->variable)) {
            auto bit_range = register_manager_.get_bit_range(variable_ref->variable->name);
            ret.get_vec().resize(bit_range.size);
            std::generate_n(ret.get_vec().begin(), bit_range.size, [bit_range, i=0]() mutable {
                return cqasm_v3x_tree::make<CqasmV3xConstInt>(
                    static_cast<cqasm_v3x_primitives::Int>(bit_range.first + i++)
                );
            });
       } else {
            return {};
        }
        return ret;
    } else if (auto index_ref = instruction_.operands[id]->as_index_ref()) {
        auto ret = index_ref->indices;
        if (is_qubit_variable(*index_ref->variable)) {
            auto qubit_range = register_manager_.get_qubit_range(index_ref->variable->name);
            std::for_each(ret.get_vec().begin(), ret.get_vec().end(), [qubit_range](const auto &index) {
                index->value += qubit_range.first;
            });
        } else if (is_bit_variable(*index_ref->variable)) {
            auto bit_range = register_manager_.get_bit_range(index_ref->variable->name);
            std::for_each(ret.get_vec().begin(), ret.get_vec().end(), [bit_range](const auto &index) {
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

[[nodiscard]] double OperandsHelper::get_float_operand(int id) const {
    return instruction_.operands[id]->as_const_float()->value;
}

[[nodiscard]] std::int64_t OperandsHelper::get_int_operand(int id) const {
    return instruction_.operands[id]->as_const_int()->value;
}

}  // namespace qx
