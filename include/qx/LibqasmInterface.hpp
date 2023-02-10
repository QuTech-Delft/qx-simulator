#pragma once

#include <memory>

namespace cqasm::v1::semantic {
class Subcircuit;
}

namespace qx {

class Circuit;

qx::Circuit loadCqasmCode(cqasm::v1::semantic::Subcircuit const &subcircuit);

} // namespace qx