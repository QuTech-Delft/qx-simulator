#include "qx/OperandsHelper.hpp"
#include "qx/V3xLibqasmInterface.hpp"
#include "qx/RegisterManager.hpp"

#include <algorithm>  // generate_n
#include <cassert>


namespace qx {

OperandsHelper::OperandsHelper(const V3Instruction &instruction, const RegisterManager &register_manager)
    : instruction_{ instruction }
    , register_manager_{ register_manager }
{}

[[nodiscard]] V3Many<V3ConstInt> OperandsHelper::get_register_operand(int id) const {
    if (auto variable_ref = instruction_.operands[id]->as_variable_ref()) {
        auto qubit_range = register_manager_.get_qubit_range(variable_ref->variable->name);
        auto ret = V3Many<V3ConstInt>{};
        ret.get_vec().resize(qubit_range.size);
        std::generate_n(ret.get_vec().begin(), qubit_range.size, [qubit_range, i=0]() mutable {
            return v3_tree::make<V3ConstInt>(static_cast<v3_primitives::Int>(qubit_range.first + i++));
        });
        return ret;
    } else if (auto index_ref = instruction_.operands[id]->as_index_ref()) {
        auto qubit_range = register_manager_.get_qubit_range(index_ref->variable->name);
        auto ret = index_ref->indices;
        std::for_each(ret.get_vec().begin(), ret.get_vec().end(), [qubit_range](const auto &index) {
            index->value += qubit_range.first;
        });
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
