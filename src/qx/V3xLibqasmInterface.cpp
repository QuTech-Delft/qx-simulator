#include "qx/V3xLibqasmInterface.hpp"


namespace qx {

bool is_qubit_variable(const V3Variable &variable) {
    return variable.typ->as_qubit() || variable.typ->as_qubit_array();
}

}  // namespace qx
