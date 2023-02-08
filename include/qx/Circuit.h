#pragma once

#include "qx/Core.h"

#include <optional>
#include <string>
#include <vector>

namespace qx {
class Circuit {
public:
    struct Measure {
        core::QubitIndex qubitIndex{};
    };

    struct MeasureAll {};

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
        std::variant<Measure, MeasureAll, PrepZ, MeasurementRegisterOperation,
                     Unitary<1>, Unitary<2>, Unitary<3>>;
    
    struct ControlledInstruction {
        ControlledInstruction(Instruction const& instr, std::shared_ptr<std::vector<core::QubitIndex>> ctrlB) : instruction(instr), controlBits(ctrlB) {};

        Instruction instruction;
        std::shared_ptr<std::vector<core::QubitIndex>> controlBits;
    };

    // We could in the future add loops and if/else...

    Circuit(std::string name = "", std::size_t iterations = 1)
        : name(name), iterations(iterations) {}

    void addInstruction(Instruction const& instruction, std::shared_ptr<std::vector<core::QubitIndex>> controlBits) {
        controlledInstructions.emplace_back(instruction, controlBits);
    }

    void execute(core::QuantumState &quantumState) const;

    std::string getName() const { return name; }

private:
    std::vector<ControlledInstruction> controlledInstructions;
    std::string const name;
    std::size_t const iterations = 1;
};
} // namespace qx