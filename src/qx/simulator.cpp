#include "qx/simulator.hpp"

#include "qx/circuit_builder.hpp"
#include "qx/cqasm_v3x.hpp"
#include "qx/random.hpp"
#include "qx/register_manager.hpp"
#include "qx/simulation_result.hpp"

// This include can not go in cqasm_v3x.hpp, due to conflicts between ANTLR and SWIG
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <iostream>
#include <optional>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/iota.hpp>
#include <variant>  // monostate

#include "libqasm/v3x/cqasm.hpp"  // default_analyzer

namespace qx {

namespace {

CqasmV3xAnalysisResult parse_cqasm_v3x_file(std::string const& file_path) {
    auto analyzer = cqasm::v3x::default_analyzer("3.0");
    return analyzer.analyze_file(file_path);
}

CqasmV3xAnalysisResult parse_cqasm_v3x_string(std::string const& program) {
    auto analyzer = cqasm::v3x::default_analyzer("3.0");
    return analyzer.analyze_string(program, std::nullopt);
}

std::variant<TreeOne<CqasmV3xProgram>, SimulationError> get_analysis_result(
    CqasmV3xAnalysisResult const& analysis_result) {
    auto const& errors = analysis_result.errors;
    if (!errors.empty()) {
        return SimulationError{ fmt::format("cQASM v3 analyzer returned errors:\n{}", fmt::join(errors, "\n")) };
    }
    auto const& program = analysis_result.root;
    if (program.empty()) {
        return SimulationError{ "cQASM v3 analyzer returned a null program" };
    }
    return program;
}

std::variant<std::monostate, SimulationResult, SimulationError> execute(
    CqasmV3xAnalysisResult const& cqasm_v3x_analysis_result, std::size_t iterations,
    std::optional<std::uint_fast64_t> seed) {
    auto analysis_result = get_analysis_result(cqasm_v3x_analysis_result);
    if (auto* error = std::get_if<SimulationError>(&analysis_result)) {
        return *error;
    }
    auto const& program = std::get<TreeOne<CqasmV3xProgram>>(analysis_result);

    if (iterations == 0) {
        return SimulationError{ "invalid number of iterations" };
    }
    if (seed) {
        random::seed(*seed);
    }

    try {
        RegisterManager::create_instance(program);
        auto circuit = Circuit{ program };
        auto simulation_iteration_accumulator =
            ranges::accumulate(ranges::views::iota(static_cast<size_t>(0), iterations),
                SimulationIterationAccumulator{},
                [&circuit](auto& acc, auto) {
                    acc.add(circuit.execute(std::monostate{}));
                    return acc;
                });
        return simulation_iteration_accumulator.get_simulation_result();
    } catch (const SimulationError& err) {
        return err;
    }
}

}  // namespace

std::variant<std::monostate, SimulationResult, SimulationError> execute_string(std::string const& program,
    std::size_t iterations, std::optional<std::uint_fast64_t> seed, std::string cqasm_version) {
    if (cqasm_version != "3.0") {
        return SimulationError{ fmt::format("unknown cQASM version: {}", cqasm_version) };
    }
    auto analysis_result = parse_cqasm_v3x_string(program);
    return execute(analysis_result, iterations, seed);
}

std::variant<std::monostate, SimulationResult, SimulationError> execute_file(std::string const& file_path,
    std::size_t iterations, std::optional<std::uint_fast64_t> seed, std::string cqasm_version) {
    if (cqasm_version != "3.0") {
        return SimulationError{ fmt::format("unknown cQASM version: {}", cqasm_version) };
    }
    auto analysis_result = parse_cqasm_v3x_file(file_path);
    return execute(analysis_result, iterations, seed);
}

}  // namespace qx
