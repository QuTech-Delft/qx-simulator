#pragma once

#include "qx/Core.hpp"  // BasisVector, BitIndex, QubitIndex
#include "qx/DenseUnitaryMatrix.hpp"
#include "qx/SimulationResult.hpp"

#include <array>
#include <cstddef>  // size_t
#include <memory> // shared_ptr
#include <vector>


namespace qx {


struct Instruction {
    virtual ~Instruction() = default;
    virtual void execute(SimulationIterationContext &context) = 0;
};


using ControlBits = std::vector<core::BitIndex>;


struct ControlledInstruction : public Instruction {
    ControlBits control_bits;
    std::shared_ptr<Instruction> instruction;

    ~ControlledInstruction() override = default;
    ControlledInstruction(ControlBits control_bits, std::shared_ptr<Instruction> instruction);
    void execute(SimulationIterationContext &context) override;
};


template <std::size_t N>
using matrix_t = core::DenseUnitaryMatrix<1 << N>;
template <std::size_t N>
using operands_t = std::array<core::QubitIndex, N>;


template <std::size_t NumberOfOperands>
struct Unitary : public Instruction {
    // Matrix is stored inline but could also be a pointer.
    matrix_t<NumberOfOperands> matrix{};
    operands_t<NumberOfOperands> operands{};

    ~Unitary() override = default;
    Unitary(matrix_t<NumberOfOperands> matrix, operands_t<NumberOfOperands> operands)
        : matrix{ std::move(matrix) }
        , operands{ std::move(operands) }
    {}
    void execute(SimulationIterationContext &context) override {
        context.state.apply(matrix, operands);
    }
};


struct NonUnitary : public Instruction {
    ~NonUnitary() override = default;
    void execute(SimulationIterationContext &context) override = 0;
};


struct Measure : public NonUnitary {
    core::QubitIndex qubitIndex{};
    core::BitIndex bitIndex{};

    ~Measure() override = default;
    Measure(core::QubitIndex const& qubitIndex, core::BitIndex const& bitIndex);
    void execute(SimulationIterationContext &context) override;
};


struct Reset : public NonUnitary {
    std::optional<core::QubitIndex> qubitIndex{};

    ~Reset() override = default;
    explicit Reset(std::optional<core::QubitIndex> qubitIndex);
    void execute(SimulationIterationContext &context) override;
};


}  // namespace qx
