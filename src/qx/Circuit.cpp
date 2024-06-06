#include "qx/Circuit.hpp"
#include "qx/GateConvertor.hpp"
#include "qx/Random.hpp"

#include <algorithm>


namespace qx {

namespace {

struct InstructionExecutor {
public:
    explicit InstructionExecutor(core::QuantumState &s) : quantumState(s){};

    void operator()(Circuit::Measure const &m) {
        quantumState.measure(m.qubitIndex, &random::randomZeroOneDouble);
    }
    void operator()(Circuit::MeasureAll const &) {
        quantumState.measureAll(&random::randomZeroOneDouble);
    }
    void operator()(Circuit::PrepZ const &r) {
        quantumState.prep(r.qubitIndex, &random::randomZeroOneDouble);
    }
    void operator()(Circuit::MeasurementRegisterOperation const &op) {
        op.operation(quantumState.getMeasurementRegister());
    }
    template <std::size_t N> void operator()(Circuit::Unitary<N> const &u) {
        quantumState.apply(u.matrix, u.operands);
    }

private:
    core::QuantumState &quantumState;
};

} // namespace

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

void Circuit::execute(core::QuantumState &quantumState, error_models::ErrorModel const &errorModel) const {
    InstructionExecutor instruction_executor(quantumState);
    for (auto const &controlled_instruction: controlled_instructions_) {
        if (auto *depolarizing_channel = std::get_if<error_models::DepolarizingChannel>(&errorModel)) {
            depolarizing_channel->addError(quantumState);
        } else {
            assert(std::get_if<std::monostate>(&errorModel) && "Unimplemented error model");
        }

        auto const &controlBits = controlled_instruction.control_bits;
        if (controlBits) {
            auto measurementRegister = quantumState.getMeasurementRegister();
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
        if (auto *measure = std::get_if<Circuit::Measure>(&instruction)) {
            instruction_executor(*measure);
        } else if (auto *measureAll = std::get_if<Circuit::MeasureAll>(&instruction)) {
            instruction_executor(*measureAll);
        } else if (auto *prepZ = std::get_if<Circuit::PrepZ>(&instruction)) {
            instruction_executor(*prepZ);
        } else if (auto *classicalOp = std::get_if<Circuit::MeasurementRegisterOperation>(&instruction)) {
            instruction_executor(*classicalOp);
        } else if (auto *instruction1 = std::get_if<Circuit::Unitary<1>>(&instruction)) {
            instruction_executor(*instruction1);
        } else if (auto *instruction2 = std::get_if<Circuit::Unitary<2>>(&instruction)) {
            instruction_executor(*instruction2);
        } else if (auto *instruction3 = std::get_if<Circuit::Unitary<3>>(&instruction)) {
            instruction_executor(*instruction3);
        } else {
            assert(false && "Unimplemented circuit instruction");
        }
    }
}

} // namespace qx
