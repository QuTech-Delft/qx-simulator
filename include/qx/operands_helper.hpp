#pragma once

#include "qx/cqasm_v3x.hpp"

#include <cstdint>  // int64_t


namespace qx {

class RegisterManager;

/*
 * OperandsHelper
 */
class OperandsHelper {
public:
    OperandsHelper(const CqasmV3xInstruction &instruction);
    [[nodiscard]] CqasmV3xIndices get_register_operand(int id) const;
    [[nodiscard]] double get_float_operand(int id) const;
    [[nodiscard]] std::int64_t get_int_operand(int id) const;
private:
    const CqasmV3xInstruction &instruction_;
};

} // namespace qx
