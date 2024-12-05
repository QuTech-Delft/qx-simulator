#pragma once

#include "qx/Core.hpp"  // BasisVector
#include "qx/ErrorModels.hpp"
#include "qx/Instructions.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SimulationResult.hpp"
#include "qx/V3xLibqasmInterface.hpp"

#include <memory>  // shared_ptr
#include <optional>
#include <string>
#include <vector>


namespace qx {

class Circuit {
    static void add_error(SimulationIterationContext &context, error_models::ErrorModel const &errorModel);

public:
    Circuit(V3OneProgram const &program, RegisterManager const &register_manager);
    void add_instruction(std::shared_ptr<Instruction> instruction);
    [[nodiscard]] SimulationIterationContext execute(error_models::ErrorModel const &errorModel) const;

public:
    V3OneProgram const &program;
    RegisterManager const &register_manager;

private:
    std::vector<std::shared_ptr<Instruction>> instructions_;
};

}  // namespace qx
