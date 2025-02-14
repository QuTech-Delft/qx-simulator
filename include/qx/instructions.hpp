#pragma once

#include <array>
#include <cstddef>  // size_t
#include <memory>  // shared_ptr
#include <vector>

#include "qx/core.hpp"  // BasisVector, BitIndex, QubitIndex
#include "qx/dense_unitary_matrix.hpp"
#include "qx/simulation_result.hpp"

namespace qx {

using ControlBits = std::vector<core::BitIndex>;

using qubit_indices_t = std::vector<core::QubitIndex>;
using bit_indices_t = std::vector<core::BitIndex>;

struct Instruction {
    virtual ~Instruction() = default;
    virtual void execute(SimulationIterationContext& context) = 0;
    [[nodiscard]] virtual qubit_indices_t get_qubit_indices() = 0;
    [[nodiscard]] virtual bit_indices_t get_bit_indices() = 0;
};

struct BitControlledInstruction : public Instruction {
    ControlBits control_bits;
    std::shared_ptr<Instruction> instruction;

    ~BitControlledInstruction() override = default;
    BitControlledInstruction(ControlBits control_bits, std::shared_ptr<Instruction> instruction);
    void execute(SimulationIterationContext& context) override;
    [[nodiscard]] qubit_indices_t get_qubit_indices() override;
    [[nodiscard]] bit_indices_t get_bit_indices() override;
};

struct Unitary : public Instruction {
    std::shared_ptr<core::matrix_t> matrix;
    std::shared_ptr<core::operands_t> operands;

    ~Unitary() override = default;
    Unitary(std::shared_ptr<core::matrix_t> matrix, std::shared_ptr<core::operands_t> operands);
    void execute(SimulationIterationContext& context) override;
    [[nodiscard]] std::shared_ptr<core::matrix_t> inverse() const;
    [[nodiscard]] std::shared_ptr<core::matrix_t> power(double exponent) const;
    [[nodiscard]] std::shared_ptr<core::matrix_t> control() const;
    [[nodiscard]] qubit_indices_t get_qubit_indices() override;
    [[nodiscard]] bit_indices_t get_bit_indices() override;
};

struct NonUnitary : public Instruction {
    ~NonUnitary() override = default;
    void execute(SimulationIterationContext& context) override = 0;
    [[nodiscard]] qubit_indices_t get_qubit_indices() override = 0;
    [[nodiscard]] bit_indices_t get_bit_indices() override = 0;
};

struct Measure : public NonUnitary {
    core::QubitIndex qubit_index{};
    core::BitIndex bit_index{};

    ~Measure() override = default;
    Measure(const core::QubitIndex& qubit_index, const core::BitIndex& bit_index);
    void execute(SimulationIterationContext& context) override;
    [[nodiscard]] qubit_indices_t get_qubit_indices() override;
    [[nodiscard]] bit_indices_t get_bit_indices() override;
};

struct Reset : public NonUnitary {
    core::QubitIndex qubit_index{};

    ~Reset() override = default;
    explicit Reset(const core::QubitIndex& qubit_index);
    void execute(SimulationIterationContext& context) override;
    [[nodiscard]] qubit_indices_t get_qubit_indices() override;
    [[nodiscard]] bit_indices_t get_bit_indices() override;
};

}  // namespace qx
