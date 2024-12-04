#include "qx/Circuit.hpp"
#include "qx/GateConvertor.hpp"
#include "qx/Instructions.hpp"
#include "qx/SimulationResult.hpp"

#include <algorithm>  // for_each


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

/* static */ void Circuit::add_error(SimulationContext &context, error_models::ErrorModel const &errorModel) {
    if (auto *depolarizing_channel = std::get_if<error_models::DepolarizingChannel>(&errorModel)) {
        depolarizing_channel->addError(context.state);
    } else if (!std::get_if<std::monostate>(&errorModel)){
        throw std::runtime_error{ "Unimplemented error model" };
    }
}

[[nodiscard]] SimulationIterationResult Circuit::execute(error_models::ErrorModel const &errorModel) const {
    auto simulation_iteration_result = SimulationIterationResult{ register_manager_ };
    std::for_each(instructions_.begin(), instructions_.end(),
        [&simulation_iteration_result, &errorModel](auto const& instruction) {
            add_error(simulation_iteration_result, errorModel);
            instruction->execute(simulation_iteration_result);
    });
    return simulation_iteration_result;
}

} // namespace qx
