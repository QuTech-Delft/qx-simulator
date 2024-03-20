#pragma once

#include <memory>


namespace cqasm::v3x::semantic {
class Program;
}

namespace qx {

class Circuit;

qx::Circuit loadCqasmCode(cqasm::v3x::semantic::Program const &v3Program);

}  // namespace qx
