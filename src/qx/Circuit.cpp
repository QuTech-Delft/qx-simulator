#include "qx/Circuit.hpp"
#include "qx/Core.hpp"
#include "qx/GateConvertor.hpp"
#include "qx/Instructions.hpp"
#include "qx/SimulationResult.hpp"

#include <algorithm>


namespace qx {

Circuit::Circuit(V3OneProgram &program, RegisterManager &register_manager)
    : program_{ program }
    , register_manager_{ register_manager } {

    GateConvertor gateConvertor{ *this };
    for (const auto &statement: program_->block->statements) {
        statement->visit(gateConvertor);
    }
}

[[nodiscard]] RegisterManager& Circuit::get_register_manager() const {
    return register_manager_;
}

void Circuit::add_instruction(std::shared_ptr<Instruction> instruction) {
    instructions_.emplace_back(std::move(instruction));
}

[[nodiscard]] SimulationIterationResult Circuit::execute(error_models::ErrorModel const &errorModel) const {
    auto simulation_iteration_result = SimulationIterationResult{ register_manager_ };
    for (auto const& instruction: instructions_) {
        if (auto *depolarizing_channel = std::get_if<error_models::DepolarizingChannel>(&errorModel)) {
            depolarizing_channel->addError(simulation_iteration_result.state);
        } else {
            assert(std::get_if<std::monostate>(&errorModel) && "Unimplemented error model");
        }
        instruction->execute(simulation_iteration_result);
    }
    return simulation_iteration_result;
}

} // namespace qx
