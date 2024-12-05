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
    Circuit() = default;
    void add_instruction(std::shared_ptr<Instruction> instruction);
    [[nodiscard]] SimulationIterationContext execute(
        RegisterManager const &registerManager,
        error_models::ErrorModel const &errorModel) const;

private:
    std::vector<std::shared_ptr<Instruction>> instructions_;
};

}  // namespace qx
