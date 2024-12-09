#include "qx/circuit.hpp"
#include "qx/circuit_builder.hpp"
#include "qx/instructions.hpp"
#include "qx/simulation_result.hpp"

#include <algorithm>  // for_each


namespace qx {

Circuit::Circuit(TreeOne<CqasmV3xProgram> const &program, RegisterManager const &register_manager)
    : program{ program }
    , register_manager{ register_manager }
{
    CircuitBuilder{ *this }.build();
}


void Circuit::add_instruction(std::shared_ptr<Instruction> instruction) {
    instructions_.emplace_back(std::move(instruction));
}

/* static */ void Circuit::add_error(SimulationIterationContext &context, error_models::ErrorModel const &error_model) {
    if (auto *depolarizing_channel = std::get_if<error_models::DepolarizingChannel>(&error_model)) {
        depolarizing_channel->add_error(context.state);
    } else if (!std::get_if<std::monostate>(&error_model)){
        throw std::runtime_error{ "unimplemented error model" };
    }
}

[[nodiscard]] SimulationIterationContext Circuit::execute(error_models::ErrorModel const &error_model) const {
    auto context = SimulationIterationContext{ register_manager };
    std::for_each(instructions_.begin(), instructions_.end(),
        [&context, &error_model](auto const& instruction) {
            add_error(context, error_model);
            instruction->execute(context);
    });
    return context;
}

} // namespace qx
