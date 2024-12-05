#include "qx/Circuit.hpp"
#include "qx/Instructions.hpp"
#include "qx/SimulationResult.hpp"

#include <algorithm>  // for_each


namespace qx {

void Circuit::add_instruction(std::shared_ptr<Instruction> instruction) {
    instructions_.emplace_back(std::move(instruction));
}

/* static */ void Circuit::add_error(SimulationIterationContext &context, error_models::ErrorModel const &errorModel) {
    if (auto *depolarizing_channel = std::get_if<error_models::DepolarizingChannel>(&errorModel)) {
        depolarizing_channel->addError(context.state);
    } else if (!std::get_if<std::monostate>(&errorModel)){
        throw std::runtime_error{ "Unimplemented error model" };
    }
}

[[nodiscard]] SimulationIterationContext Circuit::execute(
    RegisterManager const &registerManager,
    error_models::ErrorModel const &errorModel) const {
    auto context = SimulationIterationContext{ registerManager };
    std::for_each(instructions_.begin(), instructions_.end(),
        [&context, &errorModel](auto const& instruction) {
            add_error(context, errorModel);
            instruction->execute(context);
    });
    return context;
}

} // namespace qx
