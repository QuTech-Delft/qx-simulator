#include "qx/Circuit.hpp"
#include "qx/Core.hpp"
#include "qx/GateConvertor.hpp"
#include "qx/InstructionExecutor.hpp"
#include "qx/Instructions.hpp"
#include "qx/Random.hpp"

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

void Circuit::add_instruction(Instruction instruction, ControlBits control_bits) {
    controlled_instructions_.emplace_back(std::move(instruction), std::move(control_bits));
}

void Circuit::execute(core::QuantumState &quantumState, core::BasisVector &measurementRegister,
                      core::BitMeasurementRegister &bitMeasurementRegister,
                      error_models::ErrorModel const &errorModel) const {
    InstructionExecutor instruction_executor{ quantumState, measurementRegister, bitMeasurementRegister };
    for (auto const &controlled_instruction: controlled_instructions_) {
        if (auto *depolarizing_channel = std::get_if<error_models::DepolarizingChannel>(&errorModel)) {
            depolarizing_channel->addError(quantumState);
        } else {
            assert(std::get_if<std::monostate>(&errorModel) && "Unimplemented error model");
        }

        auto const &controlBits = controlled_instruction.control_bits;
        if (controlBits) {
            auto isBitNotSet = [&measurementRegister](auto const &cb) {
                return !measurementRegister.test(cb.value);
            };
            if (std::any_of(controlBits->begin(), controlBits->end(), isBitNotSet)) {
                continue;
            }
        }

        auto const &instruction = controlled_instruction.instruction;

        // AppleClang doesn't support std::visit
        // std::visit(instructionExecutor, instruction);
        if (auto *measure = std::get_if<Measure>(&instruction)) {
            instruction_executor(*measure);
        } else if (auto *reset = std::get_if<Reset>(&instruction)) {
            instruction_executor(*reset);
        } else if (auto *reset_all = std::get_if<ResetAll>(&instruction)) {
            instruction_executor(*reset_all);
        } else if (auto *classicalOp = std::get_if<MeasurementRegisterOperation>(&instruction)) {
            instruction_executor(*classicalOp);
        } else if (auto *instruction1 = std::get_if<Unitary<1>>(&instruction)) {
            instruction_executor(*instruction1);
        } else if (auto *instruction2 = std::get_if<Unitary<2>>(&instruction)) {
            instruction_executor(*instruction2);
        } else if (auto *instruction3 = std::get_if<Unitary<3>>(&instruction)) {
            instruction_executor(*instruction3);
        } else {
            assert(false && "Unimplemented circuit instruction");
        }
    }
}

} // namespace qx
