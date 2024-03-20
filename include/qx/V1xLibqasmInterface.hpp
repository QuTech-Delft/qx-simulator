#pragma once

#include <memory>


namespace cqasm::v1x::semantic {
class Subcircuit;
}

namespace qx {

class Circuit;

qx::Circuit loadCqasmCode(cqasm::v1x::semantic::Subcircuit const &subcircuit);

}  // namespace qx
