#include "qx/Simulator.hpp"

#include "qx/CircuitBuilder.hpp"
#include "qx/V3xLibqasmInterface.hpp"
#include "qx/Random.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SimulationResult.hpp"

#include "v3x/cqasm.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <iostream>
#include <optional>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/iota.hpp>
#include <variant>  // monostate


namespace qx {

using V3AnalysisResult = cqasm::v3x::analyzer::AnalysisResult;
using V3OneProgram = cqasm::v3x::ast::One<cqasm::v3x::semantic::Program>;

namespace {

V3AnalysisResult parseCqasmV3xFile(std::string const &filePath) {
    auto analyzer = cqasm::v3x::default_analyzer("3.0");
    return analyzer.analyze_file(filePath);
}

V3AnalysisResult parseCqasmV3xString(std::string const &s) {
    auto analyzer = cqasm::v3x::default_analyzer("3.0");
    return analyzer.analyze_string(s, std::nullopt);
}

std::variant<V3OneProgram, SimulationError> getV3ProgramOrError(V3AnalysisResult const& analysisResult) {
    if (!analysisResult.errors.empty()) {
        auto error = fmt::format("Cannot parse and analyze cQASM v3:\n{}", fmt::join(analysisResult.errors, "\n"));
        return SimulationError{ error };
    }
    auto program = analysisResult.root;
    assert(!program.empty());
    return program;
}

std::variant<std::monostate, SimulationResult, SimulationError> execute(
    V3AnalysisResult const& analysisResult,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed) {

    auto programOrError = getV3ProgramOrError(analysisResult);
    if (auto* error = std::get_if<SimulationError>(&programOrError)) {
        return *error;
    }
    auto program = std::get<V3OneProgram>(programOrError);
    assert(!program.empty());

    if (iterations <= 0) {
        return SimulationError{ "Invalid number of iterations" };
    }
    if (seed) {
        random::seed(*seed);
    }

    try {
        auto register_manager = RegisterManager{ program };
        auto circuit_builder = CircuitBuilder{ program, register_manager };
        auto circuit = circuit_builder.build();
        auto simulationIterationAccumulator = ranges::accumulate(
            ranges::views::iota(static_cast<size_t>(0), iterations),
            SimulationIterationAccumulator{}, [&circuit, &register_manager](auto &acc, auto) {
                acc.add(circuit.execute(register_manager, std::monostate{}));
                return acc;
            });
        return simulationIterationAccumulator.getSimulationResult(register_manager);
    } catch (const SimulationError &err) {
        return err;
    }
}

}  // namespace

std::variant<std::monostate, SimulationResult, SimulationError> executeString(
    std::string const &s,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed,
    std::string cqasm_version) {

    if (cqasm_version == "3.0") {
        auto analysisResult = parseCqasmV3xString(s);
        return execute(analysisResult, iterations, seed);
    } else {
        return SimulationError{ fmt::format("Unknown cqasm version: {}", cqasm_version) };
    }
}

std::variant<std::monostate, SimulationResult, SimulationError> executeFile(
    std::string const &filePath,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed,
    std::string cqasm_version) {

    if (cqasm_version == "3.0") {
        auto analysisResult = parseCqasmV3xFile(filePath);
        return execute(analysisResult, iterations, seed);
    } else {
        return SimulationError{ fmt::format("Unknown cqasm version: {}", cqasm_version) };
    }
}

} // namespace qx
