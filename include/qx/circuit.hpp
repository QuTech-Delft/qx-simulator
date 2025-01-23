#pragma once

#include <memory>  // shared_ptr
#include <optional>
#include <string>
#include <vector>

#include "qx/core.hpp"  // BasisVector
#include "qx/cqasm_v3x.hpp"
#include "qx/error_models.hpp"
#include "qx/instructions.hpp"
#include "qx/simulation_result.hpp"

namespace qx {

class Circuit {
    static void add_error(SimulationIterationContext& context, const error_models::ErrorModel& error_model);

public:
    Circuit(const TreeOne<CqasmV3xProgram>& program);
    void add_instruction(std::shared_ptr<Instruction> instruction);
    [[nodiscard]] SimulationIterationContext execute(const error_models::ErrorModel& error_model) const;

public:
    const TreeOne<CqasmV3xProgram>& program;

private:
    std::vector<std::shared_ptr<Instruction>> instructions_;
};

}  // namespace qx
