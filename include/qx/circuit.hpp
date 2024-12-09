#pragma once

#include "qx/core.hpp" // BasisVector
#include "qx/cqasm_v3x.hpp"
#include "qx/error_models.hpp"
#include "qx/instructions.hpp"
#include "qx/register_manager.hpp"
#include "qx/simulation_result.hpp"

#include <memory>  // shared_ptr
#include <optional>
#include <string>
#include <vector>


namespace qx {

class Circuit {
    static void add_error(SimulationIterationContext &context, error_models::ErrorModel const &error_model);

public:
    Circuit(TreeOne<CqasmV3xProgram> const &program, RegisterManager const &register_manager);
    void add_instruction(std::shared_ptr<Instruction> instruction);
    [[nodiscard]] SimulationIterationContext execute(error_models::ErrorModel const &error_model) const;

public:
    TreeOne<CqasmV3xProgram> const &program;
    RegisterManager const &register_manager;

private:
    std::vector<std::shared_ptr<Instruction>> instructions_;
};

}  // namespace qx
