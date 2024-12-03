#pragma once

#include "qx/Core.hpp"  // BasisVector
#include "qx/ErrorModels.hpp"
#include "qx/Instructions.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SimulationResult.hpp"
#include "qx/V3xLibqasmInterface.hpp"

#include <optional>
#include <string>
#include <vector>


namespace qx {

class Circuit {
public:
    Circuit(V3OneProgram &program, RegisterManager &register_manager);
    [[nodiscard]] RegisterManager& get_register_manager() const;
    void add_instruction(Instruction instruction, ControlBits control_bits);
    [[nodiscard]] SimulationIterationResult execute(error_models::ErrorModel const &errorModel) const;

private:
    V3OneProgram program_;
    RegisterManager &register_manager_;
    std::vector<ControlledInstruction> controlled_instructions_;
};

}  // namespace qx
