#include "qx/cqasm_v3x.hpp"

namespace qx {

bool is_qubit_variable(const CqasmV3xVariable &variable) {
    return variable.typ->as_qubit() || variable.typ->as_qubit_array();
}

bool is_bit_variable(const CqasmV3xVariable &variable) {
    return variable.typ->as_bit() || variable.typ->as_bit_array();
}

}  // namespace qx
