#pragma once

#include "qx/Core.hpp"  // BasisVector
#include "qx/ErrorModels.hpp"
#include "qx/Instructions.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SimulationResult.hpp"
#include "qx/V3xLibqasmInterface.hpp"

#include<memory>  // shared_ptr
#include <optional>
#include <string>
#include <vector>


namespace qx {

class Circuit {
    static void add_error(SimulationIterationContext &context, error_models::ErrorModel const &errorModel);

public:
    Circuit(V3OneProgram &program, RegisterManager &register_manager);
    [[nodiscard]] RegisterManager& get_register_manager() const;
    void add_instruction(std::shared_ptr<Instruction> instruction);
    [[nodiscard]] SimulationIterationContext execute(error_models::ErrorModel const &errorModel) const;

private:
    V3OneProgram program_;
    RegisterManager &register_manager_;
    std::vector<std::shared_ptr<Instruction>> instructions_;
};

}  // namespace qx
