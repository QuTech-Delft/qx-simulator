#include "qx/Circuit.h"

#include "qx/Random.h"

namespace qx {
namespace {
struct InstructionExecutor {
public:
    InstructionExecutor(core::QuantumState &s) : quantumState(s){};

    void operator()(Circuit::Measure const &m) {
        quantumState.measure(m.qubitIndex, [this]() { return rand.next(); });
    }
    void operator()(Circuit::MeasureAll const &) {
        quantumState.measureAll([this]() { return rand.next(); });
    }
    void operator()(Circuit::PrepZ const &r) {
        quantumState.prep(r.qubitIndex, [this]() { return rand.next(); });
    }

    template <std::size_t N> void operator()(Circuit::Unitary<N> &u) {
        if (u.controlBits) {
            auto measurementRegister = quantumState.getMeasurementRegister();
            for (auto b : *u.controlBits) {
                if (!measurementRegister.test(b.value)) {
                    return;
                }
            }
        }

        quantumState.apply(u.matrix, u.operands);
    }

private:
    core::QuantumState &quantumState;
    random::uniform_random_number_generator rand{0.0, 1.0};
};
} // namespace

void Circuit::execute(core::QuantumState &quantumState) const {
    std::size_t it = iterations;
    InstructionExecutor instructionExecutor(quantumState);
    while (it-- > 0) {
        for (auto instruction : instructions) {
            // AppleClang doesn't support std::visit
            // std::visit(instructionExecutor, instruction);
            if (auto* measure = std::get_if<Circuit::Measure>(&instruction)) {
                instructionExecutor(*measure);
            } else if (auto* measureAll = std::get_if<Circuit::MeasureAll>(&instruction)) {
                instructionExecutor(*measureAll);
            } else if (auto* prepZ = std::get_if<Circuit::PrepZ>(&instruction)) {
                instructionExecutor(*prepZ);
            } else if (auto* instruction1 = std::get_if<Circuit::Unitary<1>>(&instruction)) {
                instructionExecutor(*instruction1);
            } else if (auto* instruction2 = std::get_if<Circuit::Unitary<2>>(&instruction)) {
                instructionExecutor(*instruction2);
            } else if (auto* instruction3 = std::get_if<Circuit::Unitary<3>>(&instruction)) {
                instructionExecutor(*instruction3);
            } else {
                assert(false && "Unimplemented circuit instruction");
            }
        }
    }
}

} // namespace qx