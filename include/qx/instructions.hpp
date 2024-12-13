#pragma once

#include <array>
#include <cstddef>  // size_t
#include <memory>  // shared_ptr
#include <vector>

#include "qx/core.hpp"  // BasisVector, BitIndex, QubitIndex
#include "qx/dense_unitary_matrix.hpp"
#include "qx/simulation_result.hpp"

namespace qx {

struct Instruction {
    virtual ~Instruction() = default;
    virtual void execute(SimulationIterationContext& context) = 0;
};

using ControlBits = std::vector<core::BitIndex>;

struct ControlledInstruction : public Instruction {
    ControlBits control_bits;
    std::shared_ptr<Instruction> instruction;

    ~ControlledInstruction() override = default;
    ControlledInstruction(ControlBits control_bits, std::shared_ptr<Instruction> instruction);
    void execute(SimulationIterationContext& context) override;
};

template <std::size_t NumberOfOperands>
struct Unitary : public Instruction {
    // Matrix is stored inline but could also be a pointer.
    core::matrix_t<NumberOfOperands> matrix{};
    core::operands_t<NumberOfOperands> operands{};

    ~Unitary() override = default;
    Unitary(core::matrix_t<NumberOfOperands> matrix, core::operands_t<NumberOfOperands> operands)
    : matrix{ std::move(matrix) }
    , operands{ std::move(operands) } {}
    void execute(SimulationIterationContext& context) override { context.state.apply(matrix, operands); }
};

struct NonUnitary : public Instruction {
    ~NonUnitary() override = default;
    void execute(SimulationIterationContext& context) override = 0;
};

struct Measure : public NonUnitary {
    core::QubitIndex qubit_index{};
    core::BitIndex bit_index{};

    ~Measure() override = default;
    Measure(core::QubitIndex const& qubit_index, core::BitIndex const& bit_index);
    void execute(SimulationIterationContext& context) override;
};

struct Reset : public NonUnitary {
    std::optional<core::QubitIndex> qubit_index{};

    ~Reset() override = default;
    explicit Reset(std::optional<core::QubitIndex> qubit_index);
    void execute(SimulationIterationContext& context) override;
};

}  // namespace qx
