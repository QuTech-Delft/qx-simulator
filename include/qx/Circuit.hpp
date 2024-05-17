#pragma once

#include "qx/Core.hpp"
#include "qx/ErrorModels.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/V3xLibqasmInterface.hpp"

#include <optional>
#include <string>
#include <vector>


namespace qx {

class Circuit {
public:
    struct Measure {
        core::QubitIndex qubitIndex{};
    };
    struct MeasureAll {
    };
    struct PrepZ {
        core::QubitIndex qubitIndex{};
    };
    struct MeasurementRegisterOperation {
        std::function<void(BasisVector &)> operation;
    };
    template <std::size_t NumberOfOperands> struct Unitary {
        // Matrix is stored inline but could also be a pointer.
        core::DenseUnitaryMatrix<1 << NumberOfOperands> matrix{};
        std::array<core::QubitIndex, NumberOfOperands> operands{};
    };
    using Instruction =
        std::variant<
            Measure,
            MeasureAll,
            PrepZ,
            MeasurementRegisterOperation,
            Unitary<1>,
            Unitary<2>,
            Unitary<3>
        >;
    using ControlBits = std::shared_ptr<std::vector<core::QubitIndex>>;

    struct ControlledInstruction {
        ControlledInstruction(Instruction instruction, ControlBits control_bits)
            : instruction(std::move(instruction)), control_bits(std::move(control_bits)){};

        Instruction instruction;
        std::shared_ptr<std::vector<core::QubitIndex>> control_bits;
    };

    // We could in the future add loops and if/else...

    Circuit(V3OneProgram &program, RegisterManager &register_manager);
    RegisterManager& get_register_manager() const;
    void add_instruction(Instruction instruction, ControlBits control_bits);
    void execute(core::QuantumState &quantumState, error_models::ErrorModel const &errorModel) const;

private:
    V3OneProgram program_;
    RegisterManager &register_manager_;
    std::vector<ControlledInstruction> controlled_instructions_;
};
}  // namespace qx
