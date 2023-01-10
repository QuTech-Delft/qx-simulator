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

    template <std::size_t NumberOfOperands> struct Unitary {
        // Matrix is stored inline but could also be a pointer.
        core::DenseUnitaryMatrix<1 << NumberOfOperands> matrix{};
        std::array<core::QubitIndex, NumberOfOperands> operands{};

        // Control bits vector is stored in an optional to avoid allocation in
        // case there are no control bits.
        std::optional<std::vector<core::QubitIndex>> controlBits{};
    };

    using Instruction = std::variant<Measure, MeasureAll, PrepZ, Unitary<1>,
                                     Unitary<2>, Unitary<3>>;

    // We could in the future add loops and if/else...

    Circuit(std::string name = "", std::size_t iterations = 1)
        : name(name), iterations(iterations) {}

    void addInstruction(Instruction instruction) {
        instructions.push_back(instruction);
    }

    void execute(core::QuantumState &quantumState) const;

    std::string getName() const { return name; }

private:
    std::vector<Instruction> instructions;
    std::string const name;
    std::size_t const iterations = 1;
};
} // namespace qx