#pragma once

#include "qx/V3xLibqasmInterface.hpp"

#include <cstdint>  // int64_t


namespace qx {

class RegisterManager;

/*
 * OperandsHelper
 */
class OperandsHelper {
public:
    OperandsHelper(const V3Instruction &instruction, const RegisterManager &register_manager);
    [[nodiscard]] V3Many<V3ConstInt> get_register_operand(int id) const;
    [[nodiscard]] double get_float_operand(int id) const;
    [[nodiscard]] std::int64_t get_int_operand(int id) const;
private:
    const V3Instruction &instruction_;
    const RegisterManager &register_manager_;
};

} // namespace qx
