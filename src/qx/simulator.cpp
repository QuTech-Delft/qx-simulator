#include "qx/simulator.hpp"

#include "qx/circuit_builder.hpp"
#include "qx/cqasm_v3x.hpp"
#include "qx/random.hpp"
#include "qx/register_manager.hpp"
#include "qx/simulation_result.hpp"

// This include can not go in cqasm_v3x.hpp, due to conflicts between ANTLR and SWIG
#include "v3x/cqasm.hpp"  // default_analyzer

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <iostream>
#include <optional>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/iota.hpp>
#include <variant>  // monostate


namespace qx {

namespace {

CqasmV3xAnalysisResult parse_cqasm_v3x_file(std::string const &file_path) {
    auto analyzer = cqasm::v3x::default_analyzer("3.0");
    return analyzer.analyze_file(file_path);
}

CqasmV3xAnalysisResult parse_cqasm_v3x_string(std::string const &s) {
    auto analyzer = cqasm::v3x::default_analyzer("3.0");
    return analyzer.analyze_string(s, std::nullopt);
}

std::variant<TreeOne<CqasmV3xProgram>, SimulationError> get_analysis_result(CqasmV3xAnalysisResult const& analysis_result) {
    if (!analysis_result.errors.empty()) {
        auto error = fmt::format("Cannot parse and analyze cQASM v3:\n{}", fmt::join(analysis_result.errors, "\n"));
        return SimulationError{ error };
    }
    auto program = analysis_result.root;
    assert(!program.empty());
    return program;
}

std::variant<std::monostate, SimulationResult, SimulationError> execute(
    CqasmV3xAnalysisResult const&cqasm_v3x_analysis_result,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed) {

    auto analysis_result = get_analysis_result(cqasm_v3x_analysis_result);
    if (auto* error = std::get_if<SimulationError>(&analysis_result)) {
        return *error;
    }
    auto program = std::get<TreeOne<CqasmV3xProgram>>(analysis_result);
    assert(!program.empty());

    if (iterations <= 0) {
        return SimulationError{ "Invalid number of iterations" };
    }
    if (seed) {
        random::seed(*seed);
    }

    try {
        RegisterManager::create_instance(program);
        auto circuit = Circuit{ program };
        auto simulation_iteration_accumulator = ranges::accumulate(
            ranges::views::iota(static_cast<size_t>(0), iterations),
            SimulationIterationAccumulator{}, [&circuit](auto &acc, auto) {
                acc.add(circuit.execute(std::monostate{}));
                return acc;
            });
        return simulation_iteration_accumulator.get_simulation_result();
    } catch (const SimulationError &err) {
        return err;
    }
}

}  // namespace

std::variant<std::monostate, SimulationResult, SimulationError> execute_string(
    std::string const &s,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed,
    std::string cqasm_version) {

    if (cqasm_version == "3.0") {
        auto analysis_result = parse_cqasm_v3x_string(s);
        return execute(analysis_result, iterations, seed);
    } else {
        return SimulationError{ fmt::format("Unknown cqasm version: {}", cqasm_version) };
    }
}

std::variant<std::monostate, SimulationResult, SimulationError> execute_file(
    std::string const &file_path,
    std::size_t iterations,
    std::optional<std::uint_fast64_t> seed,
    std::string cqasm_version) {

    if (cqasm_version == "3.0") {
        auto analysis_result = parse_cqasm_v3x_file(file_path);
        return execute(analysis_result, iterations, seed);
    } else {
        return SimulationError{ fmt::format("Unknown cqasm version: {}", cqasm_version) };
    }
}

} // namespace qx
