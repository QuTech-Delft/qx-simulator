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

struct Unitary : public Instruction {
    std::shared_ptr<core::matrix_t> matrix;
    std::shared_ptr<core::operands_t> operands;

    ~Unitary() override = default;
    Unitary(std::shared_ptr<core::matrix_t> matrix, std::shared_ptr<core::operands_t> operands);
    void execute(SimulationIterationContext& context) override;
    [[nodiscard]] std::shared_ptr<core::matrix_t> inverse() const;
    [[nodiscard]] std::shared_ptr<core::matrix_t> power(double exponent) const;
    [[nodiscard]] std::shared_ptr<core::matrix_t> control() const;
};

struct NonUnitary : public Instruction {
    ~NonUnitary() override = default;
    void execute(SimulationIterationContext& context) override = 0;
};

struct Measure : public NonUnitary {
    core::QubitIndex qubit_index{};
    core::BitIndex bit_index{};

    ~Measure() override = default;
    Measure(const core::QubitIndex& qubit_index, const core::BitIndex& bit_index);
    void execute(SimulationIterationContext& context) override;
};

struct Reset : public NonUnitary {
    std::optional<core::QubitIndex> qubit_index{};

    ~Reset() override = default;
    explicit Reset(std::optional<core::QubitIndex> qubit_index);
    void execute(SimulationIterationContext& context) override;
};

}  // namespace qx
