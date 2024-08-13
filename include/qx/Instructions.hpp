#pragma once

#include "qx/Core.hpp"  // BasisVector, BitIndex, QubitIndex
#include "qx/DenseUnitaryMatrix.hpp"

#include <array>
#include <cstddef>  // size_t
#include <functional>  // function
#include <memory> // shared_ptr
#include <variant>
#include <vector>


namespace qx {

struct Measure {
    core::QubitIndex qubitIndex{};
};

struct MeasurementRegisterOperation {
    std::function<void(core::BasisVector const &)> operation;
};

template <std::size_t NumberOfOperands>
struct Unitary {
    // Matrix is stored inline but could also be a pointer.
    core::DenseUnitaryMatrix<1 << NumberOfOperands> matrix{};
    std::array<core::QubitIndex, NumberOfOperands> operands{};
};

using Instruction = std::variant<
    Measure,
    MeasurementRegisterOperation,
    Unitary<1>,
    Unitary<2>,
    Unitary<3>
>;

using ControlBits = std::shared_ptr<std::vector<core::QubitIndex>>;

struct ControlledInstruction {
    ControlledInstruction(Instruction instruction, ControlBits control_bits)
        : instruction(std::move(instruction))
        , control_bits(std::move(control_bits))
    {}

    Instruction instruction;
    ControlBits control_bits;
};

// We could in the future add loops and if/else...

}  // namespace qx
