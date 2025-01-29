#include "qx/circuit.hpp"

#include <algorithm>  // for_each

#include "qx/circuit_builder.hpp"
#include "qx/instructions.hpp"
#include "qx/simulation_result.hpp"

namespace qx {

Circuit::Circuit(const TreeOne<CqasmV3xProgram>& program)
: program{ program } {
    CircuitBuilder{ *this }.build();
}

void Circuit::add_instruction(std::shared_ptr<Instruction> instruction) {
    for (const auto& qubit_index : instruction->get_qubit_indices()) {
        RegisterManager::get_instance().set_dirty_qubit(qubit_index.value);
    }
    instructions_.emplace_back(std::move(instruction));
}

/* static */ void Circuit::add_error(SimulationIterationContext& context, const error_models::ErrorModel& error_model) {
    if (auto* depolarizing_channel = std::get_if<error_models::DepolarizingChannel>(&error_model)) {
        depolarizing_channel->add_error(context.state);
    } else if (!std::get_if<std::monostate>(&error_model)) {
        throw std::runtime_error{ "unimplemented error model" };
    }
}

[[nodiscard]] SimulationIterationContext Circuit::execute(const error_models::ErrorModel& error_model) const {
    auto context = SimulationIterationContext{};
    std::for_each(instructions_.begin(), instructions_.end(), [&context, &error_model](const auto& instruction) {
        add_error(context, error_model);
        instruction->execute(context);
    });
    return context;
}

}  // namespace qx
